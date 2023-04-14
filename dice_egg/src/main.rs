mod dice;
mod egg;
mod object;
mod text;

use dice::*;
use egg::*;
use object::*;
use raylib::prelude::*;
use text::*;

/// window title (will not be shown, running full screen)
const TITLE: &str = "window sill - basic";

/// width of a 1080p screen
const SCREEN_WIDTH: i32 = 1920;

/// height of a 1080p screen
const SCREEN_HEIGHT: i32 = 1080;

/// frames per second
const FPS: u32 = 30;

/// picture of a window sill
const SILL_IMAGE: &str = "resources/sill.png";

pub struct Game {
    pub objects: Vec<Box<dyn Object>>,
}

impl Game {
    pub fn update_objects_after_collision(&mut self, delta_time: f32) {
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

fn main() {
    let (mut rl, thread) = raylib::init()
        .size(SCREEN_WIDTH, SCREEN_HEIGHT)
        .title(TITLE)
        .fullscreen()
        .build();
    rl.set_target_fps(FPS);
    let t = rl
        .load_texture(&thread, SILL_IMAGE)
        .expect("could not load texture from window sill image");
    let t_x = SCREEN_WIDTH / 2 - t.width / 2;
    let dice = Dice::default();
    let egg1 = Egg::default();
    let mut egg2 = Egg::default();
    egg2.data.rotation = 355.0;
    egg2.data.position.x = egg1.data.position.x + 120.0;
    egg2.data.position.y = egg1.data.position.y + 40.0;
    let mut green_stone = Dice::default();
    green_stone.data.position.x = egg2.data.position.x - 210.0;
    green_stone.data.position.y = egg2.data.position.y + 50.0;
    green_stone.data.radius = dice.data.radius / 2.5;
    green_stone.data.speed = Vector2::new(0.0, 0.0);
    green_stone.data.rotation = 240.0;
    green_stone.data.rotation_speed = 0.0;
    let b1 = Box::new(dice) as Box<dyn Object>;
    let b2 = Box::new(egg1) as Box<dyn Object>;
    let b3 = Box::new(egg2) as Box<dyn Object>;
    let b4 = Box::new(green_stone) as Box<dyn Object>;
    let mut game = Game {
        objects: vec![b1, b2, b3, b4],
    };
    while !rl.window_should_close() {
        let delta_time = rl.get_frame_time();
        game.update_objects_after_collision(delta_time);
        game.draw(&mut rl, &thread, t_x, &t);
    }
}
