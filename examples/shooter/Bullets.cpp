#include "Bullets.h"
#include "..\..\features\ds_billboards.h"


void move_bullets(Bullets* bullets, float dt) {
	for (int i = 0; i < bullets->num; ++i) {
		Bullet& b = bullets->bullet[i];
		b.pos += b.velocity * ds::getElapsedSeconds();
		if (b.pos.x > 6.0f) {
			b = bullets->bullet[bullets->num-1];
			--bullets->num;
		}
	}
}

void createBulletBillboards(Bullets* bullets) {
	for (int i = 0; i < bullets->num; ++i) {
		Bullet& b = bullets->bullet[i];
		billboards::add(b.pos, ds::vec2(0.4f, 0.12f), ds::vec4(0, 0, 128, 128),0.0f,ds::vec2(1.0f),ds::Color(255,220,130,255));
	}
}
