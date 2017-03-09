#pragma once
#include "..\..\..\diesel.h"
#include "..\..\common\math.h"

// http://gizma.com/easing/
namespace tweening {

	typedef float (*TweeningType)(float,float,float,float);

	float linear(float t,float start,float end,float duration);

	float easeInQuad(float t, float start, float end, float duration);

	float easeSinus(float t, float start, float end, float duration);

	float easeOutQuad(float t, float start, float end, float duration);

	float easeInOutQuad(float t, float start, float end, float duration);

	float easeInQuart(float start,float end,float t);

	float easeInBack(float t, float start, float end, float duration);

	float easeOutBack(float t, float start, float end, float duration);

	float easeInOutBack(float t, float start, float end, float duration);

	float easeInCubic(float t, float start, float end, float duration);

	float easeOutCubic(float t, float start, float end, float duration);

	float easeInOutCubic(float t, float start, float end, float duration);

	float easeInElastic(float t, float start, float end, float duration);

	float easeOutElastic(float t, float start, float end, float duration);

	float easeInOutElastic(float t, float start, float end, float duration);

	float easeInBounce(float t, float start, float end, float d);

	float easeOutBounce(float t, float start, float end, float d);

	float easeInOutBounce(float t, float start, float end, float d);

	TweeningType get_by_index(int index);

	float interpolate(TweeningType type, float start, float end, float t, float duration);

	v2 interpolate(TweeningType type,const v2& start,const v2& end,float t,float duration);

	v3 interpolate(TweeningType type, const v3& start, const v3& end, float t, float duration);

	v4 interpolate(TweeningType type, const v4& start, const v4& end, float t, float duration);

	ds::Color interpolate(TweeningType type, const ds::Color& start, const ds::Color& end, float t, float duration);

	//void draw(TweeningType type, const ds::Texture& texture,float step = 0.1f, float delta = 1.0f);

}

