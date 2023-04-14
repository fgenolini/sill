use crate::*;
use raylib::prelude::*;

pub struct Dice {
    pub data: ObjectData,
}

impl Default for Dice {
    fn default() -> Dice {
        let mut data = ObjectData::default();
        data.collide_color = Color::GREEN;
        data.collide_color.a = 130;
        data.normal_color = Color::DARKGREEN;
        data.normal_color.a = 200;
        data.position = Vector2::new(
            SCREEN_WIDTH as f32 / 2.0 - 90.0,
            SCREEN_HEIGHT as f32 / 2.0 - 70.0,
        );
        data.speed = Vector2::new(-200.0, 140.0);
        data.radius = 50.0;
        data.rotation = 0.0;
        data.rotation_speed = 20.0;
        data.within_bounds = true;
        Dice { data }
    }
}

impl Object for Dice {
    fn draw(&self, d: &mut RaylibDrawHandle) {
        if self.data.id < 0 {
            return;
        }

        let rec = Rectangle::new(
            self.data.position.x,
            self.data.position.y,
            2.0 * self.data.radius,
            2.0 * self.data.radius,
        );
        let origin = Vector2::new(self.data.radius, self.data.radius);
        let mut color = self.data.normal_color;
        if !self.data.within_bounds {
            color = self.data.collide_color;
        }

        d.draw_rectangle_pro(rec, origin, self.data.rotation, color);
    }

    fn update(&mut self, delta_time: f32) {
        self.data.update(delta_time);
    }

    fn collision_rectangle(&self) -> Rectangle {
        self.data.collision_rectangle()
    }
}
