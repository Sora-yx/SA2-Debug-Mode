#include "stdafx.h"
#include <vector>
#include <unordered_map>
#include <algorithm>
#include "utils.h"
#include "basic-to-chunk.h"



constexpr Uint32 CHUNK_SIZE_MAX = 0xFFFF;

struct CnkTri
{
	Uint16 idx[3];
	NJS_TEX uv[3];
};

struct CnkBuild
{
	std::vector<Uint32> vlist;
	std::vector<Uint16> plist;
	NJS_CNK_MODEL model;
};

struct CnkTree
{
	std::vector<NJS_OBJECT*> nodes;
	std::vector<CnkBuild*> models;
};

std::unordered_map<const NJS_MODEL*, CnkBuild*> ModelCache;
std::unordered_map<NJS_OBJECT*, CnkTree*> TreeOwners;
std::unordered_map<const NJS_OBJECT*, NJS_OBJECT*> TreeCache;

static inline Uint32 FloatBits(Float f)
{
	Uint32 u;
	memcpy(&u, &f, sizeof(u));
	return u;
}


Uint16 StripFlags(const NJS_MATERIAL* mat, const BasicToChunkOptions& opt)
{
	Uint16 flags = 0;

	if (mat)
	{
		const Uint32 attr = mat->attrflags;

		if (attr & NJD_FLAG_IGNORE_LIGHT)
			flags |= NJD_FST_IL;

		if (attr & NJD_FLAG_IGNORE_SPECULAR)
			flags |= NJD_FST_IS;

		if (attr & NJD_FLAG_USE_ALPHA)
			flags |= NJD_FST_UA;

		if (attr & NJD_FLAG_DOUBLE_SIDE)
			flags |= NJD_FST_DB;

		if (attr & NJD_FLAG_USE_FLAT)
			flags |= NJD_FST_FL;

		if (attr & NJD_FLAG_USE_ENV)
			flags |= NJD_FST_ENV;
	}

	if (opt.forceUseAlpha)
		flags |= NJD_FST_UA;

	if (opt.forceIgnoreLight)
		flags |= NJD_FST_IL | NJD_FST_IS;

	if (opt.forceDoubleSide)
		flags |= NJD_FST_DB;

	return flags;
}


Uint16 TinyFlags(const NJS_MATERIAL* mat)
{
	Uint16 flags = 0;
	const Uint32 attr = mat->attrflags;

	if (attr & NJD_FLAG_FLIP_U)
		flags |= NJD_FFL_U;

	if (attr & NJD_FLAG_FLIP_V)
		flags |= NJD_FFL_V;

	if (attr & NJD_FLAG_CLAMP_U)
		flags |= NJD_FCL_U;

	if (attr & NJD_FLAG_CLAMP_V)
		flags |= NJD_FCL_V;

	return flags;
}

Uint16 BlendBits(const NJS_MATERIAL* mat, const BasicToChunkOptions& opt)
{
	Uint32 src = mat ? ((mat->attrflags & NJD_SA_MASK) >> 29) : 0;
	Uint32 dst = mat ? ((mat->attrflags & NJD_DA_MASK) >> 26) : 0;

	if (opt.forceBlend && src == 0 && dst == 0)
	{
		src = 4; // SRC alpha
		dst = 5; // inverse SRC alpha
	}

	return (Uint16)((src << NJD_FBS_SHIFT) | (dst << NJD_FBD_SHIFT));
}

bool BuildVertexList(const NJS_MODEL* model, std::vector<Uint32>& vlist)
{
	const Sint32 nbPoint = model->nbPoint;

	if (nbPoint <= 0 || !model->points)
		return false;

	const bool hasNormals = model->normals != nullptr;
	const Uint32 stride = hasNormals ? 6 : 3; // 32-bit words per vertex
	const Uint16 head = hasNormals ? NJD_CV_VN : NJD_CV;
	const Sint32 batchMax = (Sint32)((CHUNK_SIZE_MAX - 1) / stride);

	vlist.clear();
	vlist.reserve((size_t)nbPoint * stride + 8);

	for (Sint32 first = 0; first < nbPoint; first += batchMax)
	{
		const Sint32 count = (std::min)(nbPoint - first, batchMax);
		const Uint32 size = 1 + (Uint32)count * stride; // 32-bit words after the size field

		vlist.push_back((Uint32)head | (size << 16));
		vlist.push_back((Uint32)(Uint16)first | ((Uint32)(Uint16)count << 16));

		for (Sint32 i = first; i < first + count; i++)
		{
			const NJS_POINT3& p = model->points[i];
			vlist.push_back(FloatBits(p.x));
			vlist.push_back(FloatBits(p.y));
			vlist.push_back(FloatBits(p.z));

			if (hasNormals)
			{
				const NJS_VECTOR& n = model->normals[i];
				vlist.push_back(FloatBits(n.x));
				vlist.push_back(FloatBits(n.y));
				vlist.push_back(FloatBits(n.z));
			}
		}
	}

	vlist.push_back(NJD_ENDOFF);
	return true;
}

void AddTri(std::vector<CnkTri>& tris, Sint32 nbPoint, const NJS_TEX* uvs,
	Sint16 i0, Sint16 i1, Sint16 i2, int u0, int u1, int u2)
{
	if (i0 >= nbPoint || i1 >= nbPoint || i2 >= nbPoint || i0 < 0 || i1 < 0 || i2 < 0)
		return;

	CnkTri tri = {};
	tri.idx[0] = (Uint16)i0;
	tri.idx[1] = (Uint16)i1;
	tri.idx[2] = (Uint16)i2;

	if (uvs)
	{
		tri.uv[0] = uvs[u0];
		tri.uv[1] = uvs[u1];
		tri.uv[2] = uvs[u2];
	}

	tris.push_back(tri);
}


// convert strip to triangle
void CollectTriangles(const NJS_MESHSET& meshset, Sint32 nbPoint, std::vector<CnkTri>& tris)
{
	const Sint16* mesh = meshset.meshes;

	if (!mesh)
		return;

	const NJS_TEX* uvs = meshset.vertuv;
	const Uint16 type = meshset.type_matId & NJD_MESHSET_MASK;
	const Uint16 nbMesh = meshset.nbMesh;
	int v = 0;

	switch (type)
	{
	case NJD_MESHSET_3:
		for (Uint16 i = 0; i < nbMesh; i++, v += 3)
			AddTri(tris, nbPoint, uvs, mesh[v], mesh[v + 1], mesh[v + 2], v, v + 1, v + 2);
		break;

	case NJD_MESHSET_4:
		for (Uint16 i = 0; i < nbMesh; i++, v += 4)
		{
			AddTri(tris, nbPoint, uvs, mesh[v], mesh[v + 1], mesh[v + 2], v, v + 1, v + 2);
			AddTri(tris, nbPoint, uvs, mesh[v + 2], mesh[v + 1], mesh[v + 3], v + 2, v + 1, v + 3);
		}
		break;

	case NJD_MESHSET_N:
		for (Uint16 i = 0; i < nbMesh; i++)
		{
			const Sint16 count = mesh[v++];

			for (Sint16 j = 2; j < count; j++)
				AddTri(tris, nbPoint, uvs, mesh[v], mesh[v + j - 1], mesh[v + j], v, v + j - 1, v + j);

			v += count;
		}
		break;

	case NJD_MESHSET_TRIMESH:
		for (Uint16 i = 0; i < nbMesh; i++)
		{
			const Sint16 raw = mesh[v++];
			const bool reverse = (raw & 0x8000) != 0;
			const Sint16 count = raw & 0x7FFF;

			for (Sint16 j = 2; j < count; j++)
			{
				const int a = v + j - 2, b = v + j - 1, c = v + j;

				if (((j & 1) != 0) != reverse)
					AddTri(tris, nbPoint, uvs, mesh[b], mesh[a], mesh[c], b, a, c);
				else
					AddTri(tris, nbPoint, uvs, mesh[a], mesh[b], mesh[c], a, b, c);
			}

			v += count;
		}
		break;
	}
}

void EmitStrips(std::vector<Uint16>& plist, const std::vector<CnkTri>& tris, Uint16 head, bool withUV)
{
	const Uint32 wordsPerTri = withUV ? 10 : 4;
	const size_t batchMax = (CHUNK_SIZE_MAX - 1) / wordsPerTri;

	for (size_t first = 0; first < tris.size(); first += batchMax)
	{
		const size_t count = (std::min)(tris.size() - first, batchMax);

		plist.push_back(head);
		plist.push_back((Uint16)(1 + count * wordsPerTri));
		plist.push_back((Uint16)count);

		for (size_t t = first; t < first + count; t++)
		{
			const CnkTri& tri = tris[t];
			plist.push_back(3); // strip length, positive = normal winding

			for (int i = 0; i < 3; i++)
			{
				plist.push_back(tri.idx[i]);

				if (withUV)
				{
					plist.push_back((Uint16)tri.uv[i].u);
					plist.push_back((Uint16)tri.uv[i].v);
				}
			}
		}
	}
}

bool BuildPolygonList(const NJS_MODEL* model, const BasicToChunkOptions& opt, std::vector<Uint16>& plist)
{
	if (!model->meshsets || !model->nbMeshset)
		return false;

	plist.clear();
	std::vector<CnkTri> tris;

	for (Uint16 i = 0; i < model->nbMeshset; i++)
	{
		const NJS_MESHSET& meshset = model->meshsets[i];
		const Uint16 matId = meshset.type_matId & ~NJD_MESHSET_MASK;
		const NJS_MATERIAL* mat = (model->mats && matId < model->nbMat) ? &model->mats[matId] : nullptr;

		tris.clear();
		CollectTriangles(meshset, model->nbPoint, tris);

		if (tris.empty())
			continue;

		const bool textured = !opt.forceNoTexture && mat && (mat->attrflags & NJD_FLAG_USE_TEXTURE) != 0;
		const bool withUV = textured && meshset.vertuv != nullptr;

		if (textured)
		{
			plist.push_back(NJD_CT_TID | TinyFlags(mat));
			plist.push_back((Uint16)((mat->attr_texId & 0x1FFF) | NJD_FFM_BF));
		}

		NJS_COLOR diffuse = {};
		diffuse.argb = opt.overrideDiffuse ? opt.diffuse : (mat ? mat->diffuse.argb : opt.diffuse);

		plist.push_back(NJD_CM_D | BlendBits(mat, opt));
		plist.push_back(2); // 16-bit words after the size field
		plist.push_back((Uint16)(diffuse.color & 0xFFFF));
		plist.push_back((Uint16)(diffuse.color >> 16));

		EmitStrips(plist, tris, (withUV ? NJD_CS_UVN : NJD_CS) | StripFlags(mat, opt), withUV);
	}

	if (plist.empty())
		return false;

	plist.push_back(NJD_CE);
	return true;
}

CnkBuild* BuildModel(const NJS_MODEL* model, const BasicToChunkOptions& opt)
{
	if (!model)
		return nullptr;

	CnkBuild* build = new CnkBuild();

	if (!BuildVertexList(model, build->vlist) || !BuildPolygonList(model, opt, build->plist))
	{
		delete build;
		return nullptr;
	}

	build->model.vlist = (Sint32*)build->vlist.data();
	build->model.plist = (Sint16*)build->plist.data();
	build->model.center = model->center;
	build->model.r = model->r;
	return build;
}

NJS_OBJECT* BuildTree(const NJS_OBJECT* src, const BasicToChunkOptions& opt, CnkTree* tree,
	std::unordered_map<const NJS_MODEL*, CnkBuild*>& built)
{
	if (!src)
		return nullptr;

	NJS_OBJECT* dst = new NJS_OBJECT();
	tree->nodes.push_back(dst);

	memset(dst, 0, sizeof(NJS_OBJECT));
	dst->evalflags = src->evalflags;
	memcpy(dst->pos, src->pos, sizeof(dst->pos));
	memcpy(dst->ang, src->ang, sizeof(dst->ang));
	memcpy(dst->scl, src->scl, sizeof(dst->scl));

	if (src->model)
	{
		const NJS_MODEL* basic = (const NJS_MODEL*)src->model;
		auto cached = built.find(basic);

		if (cached == built.end())
		{
			CnkBuild* build = BuildModel(basic, opt);
			cached = built.insert({ basic, build }).first;

			if (build)
				tree->models.push_back(build);
		}

		dst->chunkmodel = cached->second ? &cached->second->model : nullptr;
	}

	dst->child = BuildTree(src->child, opt, tree, built);
	dst->sibling = BuildTree(src->sibling, opt, tree, built);
	return dst;
}

NJS_CNK_MODEL* ConvertBasicModelToChunk(const NJS_MODEL* model, const BasicToChunkOptions& opt)
{
	if (!model)
		return nullptr;

	auto cached = ModelCache.find(model);

	if (cached != ModelCache.end())
		return cached->second ? &cached->second->model : nullptr;

	CnkBuild* build = BuildModel(model, opt);
	ModelCache.insert({ model, build });

	if (!build)
	{
		PrintDebug("[SA2 Debug Mode] Basic to Chunk: model %p has no convertible geometry.\n", model);
		return nullptr;
	}

	return &build->model;
}

NJS_OBJECT* ConvertBasicObjectToChunk(const NJS_OBJECT* object, const BasicToChunkOptions& opt)
{
	if (!object)
		return nullptr;

	CnkTree* tree = new CnkTree();
	std::unordered_map<const NJS_MODEL*, CnkBuild*> built;
	NJS_OBJECT* root = BuildTree(object, opt, tree, built);

	if (!root)
	{
		delete tree;
		return nullptr;
	}

	TreeOwners.insert({ root, tree });
	return root;
}

NJS_OBJECT* GetChunkObject(const NJS_OBJECT* object, const BasicToChunkOptions& opt)
{
	if (!object)
		return nullptr;

	auto cached = TreeCache.find(object);

	if (cached != TreeCache.end())
		return cached->second;

	NJS_OBJECT* root = ConvertBasicObjectToChunk(object, opt);
	TreeCache.insert({ object, root });
	return root;
}

void ReleaseChunkModel(NJS_CNK_MODEL* model)
{
	for (auto it = ModelCache.begin(); it != ModelCache.end(); ++it)
	{
		if (it->second && &it->second->model == model)
		{
			delete it->second;
			ModelCache.erase(it);
			return;
		}
	}
}

void ReleaseChunkObject(NJS_OBJECT* object)
{
	auto owner = TreeOwners.find(object);

	if (owner == TreeOwners.end())
		return;

	for (CnkBuild* build : owner->second->models)
		delete build;

	for (NJS_OBJECT* node : owner->second->nodes)
		delete node;

	delete owner->second;
	TreeOwners.erase(owner);

	for (auto it = TreeCache.begin(); it != TreeCache.end(); ++it)
	{
		if (it->second == object)
		{
			TreeCache.erase(it);
			return;
		}
	}
}

void ReleaseAllChunkConversions()
{
	for (auto& owner : TreeOwners)
	{
		for (CnkBuild* build : owner.second->models)
			delete build;

		for (NJS_OBJECT* node : owner.second->nodes)
			delete node;

		delete owner.second;
	}

	TreeOwners.clear();
	TreeCache.clear();

	for (auto& entry : ModelCache)
		delete entry.second;

	ModelCache.clear();
}