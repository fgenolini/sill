use crate::*;
use object::*;

/// a green cube
pub struct Dice {
    pub data: ObjectData,
}

impl Dice {
    pub fn new(id: i32, radius: f32, position: Vector2, rotation: f32) -> Self {
        let mut data = ObjectData::new(id, radius, position, rotation);
        data.collide_color = Color::GREEN;
        data.collide_color.a = 130;
        data.normal_color = Color::DARKGREEN;
        data.normal_color.a = 200;
        Dice { data: data }
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
