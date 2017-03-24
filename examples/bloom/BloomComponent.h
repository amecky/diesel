#pragma once
#include "..\..\diesel.h"

struct BloomExtractSettings {
	float BloomThreshold;
	float padding1;
	float padding2;
	float padding3;
};

struct BloomSettings {
	float BloomSaturation;
	float BloomIntensity;
	float OriginalSaturation;
	float OriginalIntensity;
};

struct BlurBuffer {
	ds::matrix kernels;
	ds::matrix weights;
	ds::vec2 direction;
	ds::vec2 data;
};

class BloomComponent {

public:
	BloomComponent(RID renderTarget, RID finalRenderPass, BloomSettings* bloomSettings, BloomExtractSettings* extractSettings);
	~BloomComponent();
	void render();
private:
	void initialize();
	RID _renderTarget;
	BloomExtractSettings* _bloomExtractBuffer;
	BloomSettings* _bloomBuffer;
	BlurBuffer _blurBuffer;
	RID _finalRenderPass;
	RID _bloomRT1Pass;
	RID _bloomRT2Pass;
	//RID _backbufferPass;
	RID _extractItem;
	RID _blurrHItem;
	RID _blurrVItem;
	RID _bloomCombineItem;
};

