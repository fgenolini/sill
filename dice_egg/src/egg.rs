use crate::*;
use raylib::ffi;
use raylib::prelude::*;

pub struct Egg {
    pub data: ObjectData,
}

const H_FACTOR: f32 = 1.1;
const V_FACTOR: f32 = 0.9;

impl Default for Egg {
    fn default() -> Egg {
        let mut data = ObjectData::default();
        data.collide_color = Color::YELLOW;
        data.collide_color.a = 130;
        data.normal_color = Color::GOLD;
        data.normal_color.a = 200;
        data.position = Vector2::new(
            SCREEN_WIDTH as f32 / 2.0 + 100.0,
            SCREEN_HEIGHT as f32 / 2.0 - 10.0,
        );
        data.speed = Vector2::new(0.0, 0.0);
        data.radius = 20.0;
        data.rotation = 350.0;
        data.rotation_speed = 0.0;
        data.within_bounds = true;
        Egg { data }
    }
}

impl Object for Egg {
    fn draw(&self, d: &mut RaylibDrawHandle) {
        if self.data.id < 0 {
            return;
        }

        let mut color = self.data.normal_color;
        if !self.data.within_bounds {
            color = self.data.collide_color;
        }

        let position = self.data.position;
        if self.data.rotation != 0.0 {
            unsafe {
                ffi::rlPushMatrix();
                {
                    ffi::rlTranslatef(position.x, position.y, 0.0);
                    let mut rotation = f32::rem_euclid(self.data.rotation, 360.0);
                    if rotation < 0.0 {
                        rotation += 360.0;
                    }

                    ffi::rlRotatef(rotation, 0.0, 0.0, 1.0);
                    d.draw_ellipse(
                        0,
                        0,
                        self.data.radius * 2.0 * H_FACTOR,
                        self.data.radius * 2.0 * V_FACTOR,
                        color,
                    );
                }
                ffi::rlPopMatrix();
            }
        } else {
            d.draw_ellipse(
                position.x as i32,
                position.y as i32,
                self.data.radius * 2.0 * H_FACTOR,
                self.data.radius * 2.0 * V_FACTOR,
                color,
            );
        }
    }

    fn update(&mut self, delta_time: f32) {
        self.data.update(delta_time);
    }

    fn collision_rectangle(&self) -> Rectangle {
        self.data.collision_rectangle()
    }
}
