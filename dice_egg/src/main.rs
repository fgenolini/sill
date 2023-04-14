mod dice;
mod egg;
mod game;
mod object;
mod text;

use game::*;
use raylib::prelude::*;

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
    let mut game = Game::new();
    while !rl.window_should_close() {
        let delta_time = rl.get_frame_time();
        game.update(delta_time);
        game.draw(&mut rl, &thread, t_x, &t);
    }
}
