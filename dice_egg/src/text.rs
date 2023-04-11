use raylib::prelude::*;

pub fn draw_text(d: &mut RaylibDrawHandle) {
    d.draw_fps(10, 10);
    d.draw_text("Press escape to exit", 190, 200, 20, Color::LIGHTGRAY);
}
