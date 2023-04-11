
.PHONY: all clean run

all:
	(cd dice_egg && cargo build -r)

clean:
	(cd dice_egg && cargo clean)

run: all
	dice_egg/target/release/dice_egg

