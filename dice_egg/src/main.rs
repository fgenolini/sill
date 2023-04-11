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
    let i = Image::load_image(SILL_IMAGE).expect("could not load window sill image");
    let t = rl
        .load_texture_from_image(&thread, &i)
        .expect("could not load texture from window sill image");
    let t_x = SCREEN_WIDTH / 2 - t.width / 2;
    while !rl.window_should_close() {
        let mut d = rl.begin_drawing(&thread);
        d.clear_background(Color::WHITE);
        d.draw_texture(&t, t_x, 0, Color::WHITE);
        d.draw_fps(10, 10);
        d.draw_text("Press escape to exit", 190, 200, 20, Color::LIGHTGRAY);
    }
}
