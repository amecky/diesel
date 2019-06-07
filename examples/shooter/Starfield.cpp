#include "Starfield.h"
#include "..\..\features\ds_billboards.h"

Starfield::Starfield() {
	for (int i = 0; i < STARFIELD_LAYERS; ++i) {
		for (int j = 0; j < STARS_PER_LAYER; ++j) {
			Star& s = _stars[i * STARS_PER_LAYER + j];
			s.pos = ds::vec3(ds::random(-8.0f, 8.0f), ds::random(-4.0f, 4.0f), static_cast<float>(i));
			float v = (2.0f - static_cast<float>(i)) * 0.5f + ds::random(0.2f, 0.3f);
			s.velocity = ds::vec3(-v, 0.0f, 0.0f);
			s.layer = i;
		}
	}
}

void Starfield::moveStars(float dt) {
	for (int i = 0; i < TOTAL_STARS; ++i) {
		Star& s = _stars[i];
		s.pos += s.velocity * ds::getElapsedSeconds();
		if (s.pos.x < -10.0f) {
			s.pos.x = 10.0f;
			s.pos.y = ds::random(-8.0f, 8.0f);
			float v = (2.0f - static_cast<float>(s.layer)) * 0.5f + ds::random(0.2f, 0.3f);
			s.velocity = ds::vec3(-v, 0.0f, 0.0f);
		}
	}
}

void Starfield::createBillboards() {
	for (int i = 0; i < TOTAL_STARS; ++i) {
		billboards::add(_stars[i].pos, ds::vec2(0.05f, 0.05f), ds::vec4(0, 0, 128, 128));
	}
}
