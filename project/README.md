# Testing

This document provides practical examples of using the Forest Danger Sound Detection System.
It was designed during StuFoTech 2025, held at Kiel University.
Time implementation: 22 hours and 43 minutes
Award: Best Presentation

## Table of Contents
- [Quick Start](#quick-start)
- [Basic Workflow](#basic-workflow)
- [Advanced Usage](#advanced-usage)
- [Real-World Scenarios](#real-world-scenarios)
- [Troubleshooting](#troubleshooting)

## Quick Start

### Complete Example: 5 Minutes Setup

```bash
# 1. Build the project
make clean && make

# 2. Generate test audio samples
chmod +x generate_test_samples.sh
./generate_test_samples.sh

# 3. Create database
./forest_detector create

# 4. Add dangerous sounds
./forest_detector add samples/bear_growl.wav "Bear Growl"
./forest_detector add samples/wolf_howl.wav "Wolf Howl"
./forest_detector add samples/rattlesnake.wav "Rattlesnake"

# 5. Test matching
./forest_detector match samples/unknown_bear.wav

# 6. View database
./forest_detector list

## Basic Workflow

```bash
./forest_detector create
```

Output:
```
Database created successfully. Capacity: 100 sounds
```

This creates a `sound_database.dat` file (~108 KB) in the current directory.

### 2. Adding Sounds to Database

```bash
./forest_detector add recordings/bear1.wav "Grizzly Bear"
./forest_detector add recordings/wolf1.wav "Gray Wolf"
./forest_detector add recordings/snake1.wav "Rattlesnake"

./forest_detector add audio/dangerous/cougar_scream.wav "Mountain Lion Scream"
```

Output example:
```
Added 'Grizzly Bear' to database (samples: 88200, energy: 0.2341)
```

### 3. Listing Database Contents

```bash
./forest_detector list
```


### 4. Matching Unknown Sounds

```bash
./forest_detector match recording_unknown.wav
```

## Advanced Usage

### Building a Comprehensive Database

```bash
#!/bin/bash
# build_database.sh - Build complete forest danger database

./forest_detector create

./forest_detector add sounds/bear/grizzly_growl.wav "Grizzly Bear Growl"
./forest_detector add sounds/bear/grizzly_roar.wav "Grizzly Bear Roar"
./forest_detector add sounds/bear/black_bear_huff.wav "Black Bear Huff"

./forest_detector add sounds/wolf/gray_wolf_howl.wav "Gray Wolf Howl"
./forest_detector add sounds/wolf/wolf_bark.wav "Wolf Warning Bark"

./forest_detector add sounds/snake/rattlesnake.wav "Rattlesnake Rattle"
./forest_detector add sounds/snake/copperhead_hiss.wav "Copperhead Hiss"

./forest_detector add sounds/cat/mountain_lion.wav "Mountain Lion Scream"
./forest_detector add sounds/cat/bobcat_scream.wav "Bobcat Scream"

./forest_detector add sounds/other/wild_boar.wav "Wild Boar Charge"
./forest_detector add sounds/other/bee_swarm.wav "Bee Swarm"
./forest_detector add sounds/other/avalanche.wav "Avalanche"

echo "Database built with $(./forest_detector list | grep 'Current count' | cut -d: -f2) sounds"
```

### Batch Processing Multiple Files

```bash
#!/bin/bash
# batch_match.sh - Match multiple unknown sounds

for file in unknown/*.wav; do
    echo "============================================"
    echo "Processing: $file"
    echo "============================================"
    ./forest_detector match "$file"
    echo ""
done
```

### Database Backup and Restore

```bash
cp sound_database.dat backups/database_$(date +%Y%m%d).dat

cp backups/database_20250115.dat sound_database.dat

./forest_detector list > database_contents.txt
```
