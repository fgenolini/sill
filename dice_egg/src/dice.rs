use crate::*;
use raylib::prelude::*;

pub struct Dice {
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

impl Default for Dice {
    fn default() -> Dice {
        let id = 0;
        let mut collide_color = Color::GREEN;
        collide_color.a = 130;
        let mut normal_color = Color::DARKGREEN;
        normal_color.a = 200;
        let position = Vector2::new(
            SCREEN_WIDTH as f32 / 2.0 - 90.0,
            SCREEN_HEIGHT as f32 / 2.0 - 70.0,
        );
        let speed = Vector2::new(-200.0, 140.0);
        let radius = 50.0;
        let rotation = 0.0;
        let rotation_speed = 20.0;
        let within_bounds = true;
        Dice {
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
}

impl Dice {
    pub fn draw(&self, d: &mut RaylibDrawHandle) {
        if self.id < 0 {
            return;
        }

        let rec = Rectangle::new(
            self.position.x,
            self.position.y,
            2.0 * self.radius,
            2.0 * self.radius,
        );
        let origin = Vector2::new(self.radius, self.radius);
        let mut color = self.normal_color;
        if !self.within_bounds {
            color = self.collide_color;
        }
        d.draw_rectangle_pro(rec, origin, self.rotation, color);
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
