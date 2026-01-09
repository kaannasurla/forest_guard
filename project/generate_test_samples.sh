#!/bin/bash
# Generate test WAV files using Sox (Sound eXchange)
# Install: sudo apt-get install sox  (Ubuntu)


set -e

OUTPUT_DIR="samples"
DURATION=2
SAMPLE_RATE=44100

# Colors
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

echo "========================================"
echo "Generating Test Audio Samples"
echo "========================================"
echo ""

# Check if sox is installed
if ! command -v sox > /dev/null 2>&1; then
	echo "Error: sox is not installed"
	echo "Install with:"
	echo "  Ubuntu: sudo apt-get install sox"
	echo "  macOS:  brew install sox"
	exit 1
fi

# Create output directory
mkdir -p "$OUTPUT_DIR"

# Function to generate a sound
generate_sound() {
	local filename="$1"
	local description="$2"
	shift 2
	local sox_args=("$@")

	echo -e "${YELLOW}Generating:${NC} $description"
	echo "Command: sox -n -r $SAMPLE_RATE -c 1 -b 16 $OUTPUT_DIR/$filename ${sox_args[@]} trim 0 $DURATION"
	sox -n -r "$SAMPLE_RATE" -c 1 -b 16 "$OUTPUT_DIR/$filename" \
		"${sox_args[@]}" trim 0 "$DURATION"
	echo -e "${GREEN}Created:${NC} $OUTPUT_DIR/$filename"
	echo ""
}

# Generate various test sounds

# 1. Low frequency growl (bear-like)
generate_sound "bear_growl.wav" "Bear Growl (Low frequency rumble)" \
	synth 2 sine 80 tremolo 5 50 lowpass 500 vol 0.7

# 2. High frequency howl (wolf-like)
generate_sound "wolf_howl.wav" "Wolf Howl (Rising pitch)" \
	synth 2 sine 600:800 tremolo 6 40 vol 0.6

# 3. Rattlesnake rattle (high frequency noise)
generate_sound "rattlesnake.wav" "Rattlesnake Rattle (Fast clicking)" \
	synth 2 whitenoise highpass 2000 tremolo 60 90 vol 0.3

# 4. Mountain lion scream (high pitched)
generate_sound "mountain_lion.wav" "Mountain Lion Scream (High pitched wail)" \
	synth 2 sine 1000 tremolo 4 60 vol 0.5

# 5. Wild boar snort (mid-range grunt)
generate_sound "wild_boar.wav" "Wild Boar Charge (Grunting snorts)" \
	synth 2 square 250 tremolo 10 80 lowpass 800 vol 0.4

# 6. Hawk screech (sharp high frequency)
generate_sound "hawk_screech.wav" "Hawk Screech (Sharp cry)" \
	synth 1 sine 2000 tremolo 8 70 vol 0.5

# 7. Thunder/storm (for testing non-animal sounds)
generate_sound "thunder.wav" "Thunder (Deep rumble)" \
	synth 2 brownnoise lowpass 200 tremolo 0.5 50 vol 0.8

# 8. Branch breaking (crack sound)
generate_sound "branch_break.wav" "Branch Breaking (Snap)" \
	synth 0.5 whitenoise highpass 800 lowpass 4000 fade t 0 0.5 0.4 vol 0.4

# 9. Bee swarm (buzzing)
generate_sound "bee_swarm.wav" "Bee Swarm (Buzzing)" \
	synth 2 square 220 tremolo 200 90 vol 0.3

# 10. Test tone (for calibration)
generate_sound "test_tone.wav" "Test Tone (1000 Hz)" \
	synth 2 sine 1000 vol 0.5

# Generate a similar sound for testing matching
echo -e "${YELLOW}Generating:${NC} Similar to bear growl (for match testing)"
sox -n -r "$SAMPLE_RATE" -c 1 -b 16 "$OUTPUT_DIR/unknown_bear.wav" \
	synth 2 sine 90 tremolo 5 48 lowpass 480 trim 0 "$DURATION" vol 0.7
echo -e "${GREEN}Created:${NC} $OUTPUT_DIR/unknown_bear.wav"
echo ""

# Generate a different sound for testing non-match
echo -e "${YELLOW}Generating:${NC} Bird chirp (for non-match testing)"
sox -n -r "$SAMPLE_RATE" -c 1 -b 16 "$OUTPUT_DIR/bird_chirp.wav" \
	synth 0.2 sine 3000 repeat 5 trim 0 "$DURATION" vol 0.4
echo -e "${GREEN}Created:${NC} $OUTPUT_DIR/bird_chirp.wav"
echo ""

echo "========================================"
echo "Summary"
echo "========================================"
echo "Generated $(ls -1 $OUTPUT_DIR/*.wav 2>/dev/null | wc -l) WAV files in $OUTPUT_DIR/"
echo ""
echo "Dangerous sounds (add to database):"
echo "  - bear_growl.wav"
echo "  - wolf_howl.wav"
echo "  - rattlesnake.wav"
echo "  - mountain_lion.wav"
echo "  - wild_boar.wav"
echo "  - hawk_screech.wav"
echo "  - thunder.wav"
echo "  - branch_break.wav"
echo "  - bee_swarm.wav"
echo ""
echo "Test sounds:"
echo "  - unknown_bear.wav (should match bear_growl)"
echo "  - bird_chirp.wav (should not match anything)"
echo "  - test_tone.wav (calibration)"
echo ""
echo "Next steps:"
echo "  1. ./forest_detector create"
echo "  2. ./forest_detector add samples/bear_growl.wav \"Bear Growl\""
echo "  3. ./forest_detector match samples/unknown_bear.wav"
