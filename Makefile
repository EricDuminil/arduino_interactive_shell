all:
	pio -f -c vim run

upload:
	pio -f -c vim run --target upload -e $(board)

clean:
	pio -f -c vim run --target clean

update:
	pio -f -c vim update

monitor:
	pio device monitor --filter colorize
