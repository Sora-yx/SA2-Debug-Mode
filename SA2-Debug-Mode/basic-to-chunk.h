#pragma once

struct BasicToChunkOptions
{
	bool forceUseAlpha = false;
	bool forceIgnoreLight = false;
	bool forceDoubleSide = false;
	bool forceNoTexture = false;
	bool forceBlend = false;
	bool overrideDiffuse = false;
	NJS_BGRA diffuse = { 0xFF, 0xFF, 0xFF, 0xFF };
};

NJS_CNK_MODEL* ConvertBasicModelToChunk(const NJS_MODEL* model, const BasicToChunkOptions& opt);
NJS_OBJECT* ConvertBasicObjectToChunk(const NJS_OBJECT* object, const BasicToChunkOptions& opt);
NJS_OBJECT* GetChunkObject(const NJS_OBJECT* object, const BasicToChunkOptions& opt);
void ReleaseChunkModel(NJS_CNK_MODEL* model);
void ReleaseChunkObject(NJS_OBJECT* object);
void ReleaseAllChunkConversions();