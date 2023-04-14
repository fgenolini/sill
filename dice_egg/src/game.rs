use crate::*;
use dice::*;
use egg::*;
use object::*;
use text::*;

/// 2D game of moving objects on a window sill
pub struct Game {
    pub objects: Vec<Box<dyn Object>>,
}

impl Game {
    /// build up a 2D scene composed of multiple objects
    pub fn new() -> Self {
        let mut dice = Dice::new(
            0,
            50.0,
            Vector2::new(
                SCREEN_WIDTH as f32 / 2.0 - 90.0,
                SCREEN_HEIGHT as f32 / 2.0 - 70.0,
            ),
            0.0,
        );
        dice.data.speed = Vector2::new(-200.0, 140.0);
        dice.data.rotation_speed = 20.0;
        let egg1 = Egg::new(
            1,
            20.0,
            Vector2::new(
                SCREEN_WIDTH as f32 / 2.0 + 100.0,
                SCREEN_HEIGHT as f32 / 2.0 - 10.0,
            ),
            350.0,
        );
        let egg2 = Egg::new(
            2,
            20.0,
            Vector2::new(egg1.data.position.x + 120.0, egg1.data.position.y + 40.0),
            355.0,
        );
        let green_stone = Dice::new(
            3,
            dice.data.radius / 2.5,
            Vector2::new(egg2.data.position.x - 210.0, egg2.data.position.y + 50.0),
            240.0,
        );
        let b1 = Box::new(dice) as Box<dyn Object>;
        let b2 = Box::new(egg1) as Box<dyn Object>;
        let b3 = Box::new(egg2) as Box<dyn Object>;
        let b4 = Box::new(green_stone) as Box<dyn Object>;
        Game {
            objects: vec![b1, b2, b3, b4],
        }
    }

    /// change the objects (position, speed, rotation and colour)
    pub fn update(&mut self, delta_time: f32) {
        for object in self.objects.iter_mut() {
            object.update(delta_time);
        }
    }

    pub fn draw(&self, rl: &mut RaylibHandle, thread: &RaylibThread, t_x: i32, t: &Texture2D) {
        let mut d = rl.begin_drawing(&thread);
        d.clear_background(Color::WHITE);
        self.draw_contents(&mut d, t_x, &t);
    }

    fn draw_contents(&self, d: &mut RaylibDrawHandle, t_x: i32, t: &Texture2D) {
        d.draw_texture(&t, t_x, 0, Color::WHITE);
        self.draw_objects(d);
        draw_text(d);
    }

    fn draw_objects(&self, d: &mut RaylibDrawHandle) {
        for object in self.objects.iter() {
            object.draw(d);
        }
    }
}
