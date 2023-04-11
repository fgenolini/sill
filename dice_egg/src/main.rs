mod dice;
mod text;

use dice::*;
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
    let mut dice = Dice::default();
    while !rl.window_should_close() {
        let delta_time = rl.get_frame_time();
        update_objects_after_collision(delta_time, &mut dice);
        draw(&mut rl, &thread, t_x, &t, &mut dice);
    }
}

fn update_objects_after_collision(delta_time: f32, dice_0: &mut Dice) {
    dice_0.update(delta_time);
}

fn draw(rl: &mut RaylibHandle, thread: &RaylibThread, t_x: i32, t: &Texture2D, dice_0: &mut Dice) {
    let mut d = rl.begin_drawing(&thread);
    d.clear_background(Color::WHITE);
    draw_contents(&mut d, t_x, &t, dice_0);
}

fn draw_contents(d: &mut RaylibDrawHandle, t_x: i32, t: &Texture2D, dice_0: &mut Dice) {
    d.draw_texture(&t, t_x, 0, Color::WHITE);
    draw_objects(d, dice_0);
    draw_text(d);
}

fn draw_objects(d: &mut RaylibDrawHandle, dice_0: &mut Dice) {
    dice_0.draw(d);
}
