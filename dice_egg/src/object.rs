use crate::*;

pub struct ObjectData {
    pub id: i32,
    pub collide_color: Color,
    pub normal_color: Color,
    pub position: Vector2,
    pub speed: Vector2,
    pub radius: f32,
    pub rotation: f32,
    pub rotation_speed: f32,
    pub within_bounds: bool,
}

const EXTRA_MARGIN: f32 = 1.5;

impl ObjectData {
    pub fn new(id: i32, radius: f32, position: Vector2, rotation: f32) -> Self {
        let collide_color = Color::BLUE;
        let normal_color = Color::DARKBLUE;
        let speed = Vector2::new(0.0, 0.0);
        let rotation_speed = 0.0;
        let within_bounds = true;
        ObjectData {
            id,
            collide_color,
            normal_color,
            position,
            speed,
            radius,
            rotation,
            rotation_speed,
            within_bounds,
        }
    }

    pub fn collision_rectangle(&self) -> Rectangle {
        let mut extra = EXTRA_MARGIN;
        if self.rotation.abs() < 10.0 {
            extra = 0.0;
        }

        let mut rec = Rectangle::default();
        rec.x = self.position.x - extra;
        rec.width = self.radius * 2.0 + 2.0 * extra;
        rec.y = self.position.y - extra;
        rec.height = self.radius * 2.0 + 2.0 * extra;
        rec
    }

    pub fn update(&mut self, delta_time: f32) {
        self.position.x += self.speed.x * delta_time;
        self.position.y += self.speed.y * delta_time;
        self.rotation += self.rotation_speed * delta_time;
        self.rotation = f32::rem_euclid(self.rotation, 360.0);
        if self.rotation < 0.0 {
            self.rotation += 360.0;
        }

        if !self.out_of_bounds() {
            self.within_bounds = true;
            return;
        }

        self.within_bounds = false;
        if self.rotation_speed.abs() < 10.0 {
            self.rotation_speed = 10.0;
        } else {
            self.rotation_speed *= 1.01;
        }

        if self.out_of_bounds_x() {
            let left_edge = self.radius + 1.0;
            let right_edge = SCREEN_WIDTH as f32 - self.radius - 1.0;
            let positive_speed = self.speed.x > 0.0;
            if self.position.x < left_edge {
                self.position.x += 4.0;
                if self.position.x < -self.radius {
                    self.position.x = 1.0;
                }

                if !positive_speed {
                    self.speed.x *= -1.0;
                }
            } else if self.position.x > right_edge {
                self.position.x -= 4.0;
                if self.position.x > SCREEN_WIDTH as f32 + self.radius {
                    self.position.x = SCREEN_WIDTH as f32 - 1.0;
                }

                if positive_speed {
                    self.speed.x *= -1.0;
                }
            }

            self.speed.x *= 0.99;
        }

        if self.out_of_bounds_y() {
            let bottom_edge = SCREEN_HEIGHT as f32 - self.radius - 1.0;
            let top_edge = self.radius + 1.0;
            let positive_speed = self.speed.y > 0.0;
            if self.position.y < top_edge {
                self.position.y += 4.0;
                if self.position.y < -self.radius {
                    self.position.y = 1.0;
                }

                if !positive_speed {
                    self.speed.y *= -1.0;
                }
            } else if self.position.y > bottom_edge {
                self.position.y -= 4.0;
                if self.position.y > SCREEN_HEIGHT as f32 + self.radius {
                    self.position.y = SCREEN_HEIGHT as f32 - 1.0;
                }

                if positive_speed {
                    self.speed.y *= -1.0;
                }
            }

            self.speed.y *= 0.99;
        }
    }

    fn out_of_bounds(&self) -> bool {
        self.out_of_bounds_x() || self.out_of_bounds_y()
    }

    fn out_of_bounds_x(&self) -> bool {
        self.position.x < self.radius || self.position.x > SCREEN_WIDTH as f32 - self.radius
    }

    fn out_of_bounds_y(&self) -> bool {
        self.position.y < self.radius || self.position.y > SCREEN_HEIGHT as f32 - self.radius
    }
}

pub trait Object {
    fn draw(&self, d: &mut RaylibDrawHandle);
    fn update(&mut self, delta_time: f32);
    fn collision_rectangle(&self) -> Rectangle;
}
