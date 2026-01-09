/*
 * Forest Danger Sound Detection System
 * Uses low-level programming techniques to create a sound fingerprint
 * 
 * Author: Kaan Nasurla
 * Country: Romania
 */

#define _POSIX_C_SOURCE 200112L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdint.h>

#define DATABASE_FILE "sound_database.dat"
#define MAX_SAMPLES 100
#define SAMPLE_RATE 44100
#define FINGERPRINT_SIZE 256
#define CHUNK_SIZE 4096

typedef struct {
    char name[64];
    float fingerprint[FINGERPRINT_SIZE];
    uint32_t sample_count;
    float energy;
} SoundFingerprint;

typedef struct {
    uint32_t magic;
    uint32_t version;
    uint32_t count;
    uint32_t max_samples;
} DatabaseHeader;

#define DB_MAGIC 0x464F5253

void create_database();
void add_sound_to_database(const char *filepath, const char *name);
void match_sound(const char *filepath);
void list_database();
float* read_wav_file(const char *filepath, uint32_t *sample_count);
void compute_fingerprint(float *audio_data, uint32_t sample_count, float *fingerprint);
float compare_fingerprints(float *fp1, float *fp2);
void *map_file_readonly(const char *filepath, size_t *size);
void *map_file_readwrite(const char *filepath, size_t *size);

void create_database() {
    int fd = open(DATABASE_FILE, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("Failed to create database file");
        exit(1);
    }
    
    size_t db_size = sizeof(DatabaseHeader) + 
                     sizeof(SoundFingerprint) * MAX_SAMPLES;
    
    if (ftruncate(fd, db_size) == -1) {
        perror("Failed to extend file");
        close(fd);
        exit(1);
    }
    
    void *mapped = mmap(NULL, db_size, PROT_READ | PROT_WRITE, 
                        MAP_SHARED, fd, 0);
    if (mapped == MAP_FAILED) {
        perror("mmap failed");
        close(fd);
        exit(1);
    }
    
    DatabaseHeader *header = (DatabaseHeader *)mapped;
    header->magic = DB_MAGIC;
    header->version = 1;
    header->count = 0;
    header->max_samples = MAX_SAMPLES;
    
    if (msync(mapped, db_size, MS_SYNC) == -1) {
        perror("msync failed");
    }
    
    if (munmap(mapped, db_size) == -1) {
        perror("munmap failed");
    }
    
    close(fd);
    printf("Database created successfully. Capacity: %d sounds\n", MAX_SAMPLES);
}

void *map_file_readonly(const char *filepath, size_t *size) {
    int fd = open(filepath, O_RDONLY);
    if (fd == -1) {
        perror("Failed to open file");
        return NULL;
    }
    
    struct stat sb;
    if (fstat(fd, &sb) == -1) {
        perror("fstat failed");
        close(fd);
        return NULL;
    }
    
    *size = sb.st_size;
    
    void *mapped = mmap(NULL, *size, PROT_READ, MAP_PRIVATE, fd, 0);
    close(fd);
    
    if (mapped == MAP_FAILED) {
        perror("mmap failed");
        return NULL;
    }
    
    if (mprotect(mapped, *size, PROT_READ) == -1) {
        perror("mprotect failed");
        munmap(mapped, *size);
        return NULL;
    }
    
    return mapped;
}

void *map_file_readwrite(const char *filepath, size_t *size) {
    int fd = open(filepath, O_RDWR);
    if (fd == -1) {
        perror("Failed to open database file");
        return NULL;
    }
    
    struct stat sb;
    if (fstat(fd, &sb) == -1) {
        perror("fstat failed");
        close(fd);
        return NULL;
    }
    
    *size = sb.st_size;
    
    void *mapped = mmap(NULL, *size, PROT_READ | PROT_WRITE, 
                        MAP_SHARED, fd, 0);
    close(fd);
    
    if (mapped == MAP_FAILED) {
        perror("mmap failed");
        return NULL;
    }
    
    return mapped;
}

float* read_wav_file(const char *filepath, uint32_t *sample_count) {
    size_t file_size;
    void *mapped = map_file_readonly(filepath, &file_size);
    
    if (!mapped) {
        return NULL;
    }
    
    size_t header_size = 44;
    if (file_size < header_size) {
        munmap(mapped, file_size);
        fprintf(stderr, "File too small\n");
        return NULL;
    }

    size_t data_size = file_size - header_size;
    *sample_count = data_size / 2;

    float *samples = malloc(*sample_count * sizeof(float));
    if (!samples) {
        munmap(mapped, file_size);
        return NULL;
    }

    int16_t *pcm_data = (int16_t *)((char *)mapped + header_size);
    for (uint32_t i = 0; i < *sample_count; i++) {
        samples[i] = pcm_data[i] / 32768.0f;
    }

    munmap(mapped, file_size);
    return samples;
}

void compute_fingerprint(float *audio_data, uint32_t sample_count, 
                        float *fingerprint) {
    memset(fingerprint, 0, FINGERPRINT_SIZE * sizeof(float));
    
    uint32_t chunks = FINGERPRINT_SIZE / 4;
    uint32_t chunk_size = sample_count / chunks;

    if (chunk_size == 0) chunk_size = 1;

    for (uint32_t i = 0; i < chunks && i * chunk_size < sample_count; i++) {
        uint32_t start = i * chunk_size;
        uint32_t end = start + chunk_size;
        if (end > sample_count) end = sample_count;

        float energy = 0;
        for (uint32_t j = start; j < end; j++) {
            energy += audio_data[j] * audio_data[j];
        }
        fingerprint[i * 4] = sqrtf(energy / (end - start));

        uint32_t zcr = 0;
        for (uint32_t j = start + 1; j < end; j++) {
            if ((audio_data[j] >= 0 && audio_data[j-1] < 0) ||
                (audio_data[j] < 0 && audio_data[j-1] >= 0)) {
                zcr++;
            }
        }
        fingerprint[i * 4 + 1] = (float)zcr / (end - start);

        float mean = 0;
        for (uint32_t j = start; j < end; j++) {
            mean += audio_data[j];
        }
        fingerprint[i * 4 + 2] = mean / (end - start);

        float variance = 0;
        mean = fingerprint[i * 4 + 2];
        for (uint32_t j = start; j < end; j++) {
            float diff = audio_data[j] - mean;
            variance += diff * diff;
        }
        fingerprint[i * 4 + 3] = sqrtf(variance / (end - start));
    }
}

float compare_fingerprints(float *fp1, float *fp2) {
    float distance = 0;
    float norm1 = 0, norm2 = 0;

    for (int i = 0; i < FINGERPRINT_SIZE; i++) {
        distance += fp1[i] * fp2[i];
        norm1 += fp1[i] * fp1[i];
        norm2 += fp2[i] * fp2[i];
    }
    
    if (norm1 == 0 || norm2 == 0) return 0;
    
    float similarity = distance / (sqrtf(norm1) * sqrtf(norm2));
    return (similarity + 1.0f) / 2.0f;
}

void add_sound_to_database(const char *filepath, const char *name) {
    uint32_t sample_count;
    float *audio_data = read_wav_file(filepath, &sample_count);

    if (!audio_data) {
        fprintf(stderr, "Failed to read audio file: %s\n", filepath);
        return;
    }

    float fingerprint[FINGERPRINT_SIZE];
    compute_fingerprint(audio_data, sample_count, fingerprint);

    float energy = 0;
    for (uint32_t i = 0; i < sample_count; i++) {
        energy += audio_data[i] * audio_data[i];
    }
    energy = sqrtf(energy / sample_count);

    free(audio_data);

    size_t db_size;
    void *mapped = map_file_readwrite(DATABASE_FILE, &db_size);
    if (!mapped) {
        fprintf(stderr, "Failed to open database\n");
        return;
    }

    DatabaseHeader *header = (DatabaseHeader *)mapped;

    if (header->magic != DB_MAGIC) {
        fprintf(stderr, "Invalid database file\n");
        munmap(mapped, db_size);
        return;
    }

    if (header->count >= header->max_samples) {
        fprintf(stderr, "Database is full\n");
        munmap(mapped, db_size);
        return;
    }

    SoundFingerprint *sounds = (SoundFingerprint *)((char *)mapped + 
                                                    sizeof(DatabaseHeader));
    SoundFingerprint *new_sound = &sounds[header->count];
    
    strncpy(new_sound->name, name, sizeof(new_sound->name) - 1);
    new_sound->name[sizeof(new_sound->name) - 1] = '\0';
    memcpy(new_sound->fingerprint, fingerprint, sizeof(fingerprint));
    new_sound->sample_count = sample_count;
    new_sound->energy = energy;
    header->count++;

    msync(mapped, db_size, MS_SYNC);

    munmap(mapped, db_size);

    printf("Added '%s' to database (samples: %u, energy: %.4f)\n", 
           name, sample_count, energy);
}

void match_sound(const char *filepath) {
    uint32_t sample_count;
    float *audio_data = read_wav_file(filepath, &sample_count);

    if (!audio_data) {
        fprintf(stderr, "Failed to read audio file: %s\n", filepath);
        return;
    }

    float query_fingerprint[FINGERPRINT_SIZE];
    compute_fingerprint(audio_data, sample_count, query_fingerprint);
    free(audio_data);

    size_t db_size;
    void *mapped = map_file_readonly(DATABASE_FILE, &db_size);
    if (!mapped) {
        fprintf(stderr, "Failed to open database\n");
        return;
    }

    DatabaseHeader *header = (DatabaseHeader *)mapped;

    if (header->magic != DB_MAGIC) {
        fprintf(stderr, "Invalid database file\n");
        munmap(mapped, db_size);
        return;
    }

    printf("\n=== Matching against %u sounds in database ===\n\n", 
           header->count);

    SoundFingerprint *sounds = (SoundFingerprint *)((char *)mapped + 
                                                    sizeof(DatabaseHeader));

    float best_score = 0;
    int best_match = -1;

    for (uint32_t i = 0; i < header->count; i++) {
        float score = compare_fingerprints(query_fingerprint, 
                                          sounds[i].fingerprint);

        printf("%-30s | Similarity: %.2f%%\n", sounds[i].name, score * 100);

        if (score > best_score) {
            best_score = score;
            best_match = i;
        }
    }

    printf("\n");
    if (best_match >= 0 && best_score > 0.7) {
        printf("🚨 MATCH FOUND! 🚨\n");
        printf("Dangerous sound detected: %s\n", sounds[best_match].name);
        printf("Confidence: %.2f%%\n", best_score * 100);
    } else if (best_match >= 0) {
        printf("Possible match: %s (%.2f%% confidence)\n", 
               sounds[best_match].name, best_score * 100);
        printf("Confidence too low for definitive match.\n");
    } else {
        printf("No matches found in database.\n");
    }

    munmap(mapped, db_size);
}

void list_database() {
    size_t db_size;
    void *mapped = map_file_readonly(DATABASE_FILE, &db_size);
    
    if (!mapped) {
        fprintf(stderr, "Failed to open database\n");
        return;
    }

    DatabaseHeader *header = (DatabaseHeader *)mapped;

    if (header->magic != DB_MAGIC) {
        fprintf(stderr, "Invalid database file\n");
        munmap(mapped, db_size);
        return;
    }

    printf("\n=== Forest Danger Sound Database ===\n");
    printf("Version: %u\n", header->version);
    printf("Capacity: %u\n", header->max_samples);
    printf("Current count: %u\n\n", header->count);

    if (header->count == 0) {
        printf("Database is empty.\n");
    } else {
        SoundFingerprint *sounds = (SoundFingerprint *)((char *)mapped + 
                                                        sizeof(DatabaseHeader));
        
        printf("%-4s | %-30s | %-12s | %-10s\n", 
               "ID", "Name", "Samples", "Energy");
        printf("-----+--------------------------------+--------------+------------\n");
        
        for (uint32_t i = 0; i < header->count; i++) {
            printf("%-4u | %-30s | %-12u | %-10.4f\n",
                   i + 1, sounds[i].name, sounds[i].sample_count, 
                   sounds[i].energy);
        }
    }

    munmap(mapped, db_size);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Forest Danger Sound Detection System\n\n");
        printf("Usage:\n");
        printf("  %s create              - Create new database\n", argv[0]);
        printf("  %s add <file> <name>   - Add sound to database\n", argv[0]);
        printf("  %s match <file>        - Match sound against database\n", argv[0]);
        printf("  %s list                - List all sounds in database\n", argv[0]);
        printf("\nExample:\n");
        printf("  %s create\n", argv[0]);
        printf("  %s add bear_growl.wav \"Bear Growl\"\n", argv[0]);
        printf("  %s match unknown.wav\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "create") == 0) {
        create_database();
    } else if (strcmp(argv[1], "add") == 0) {
        if (argc < 4) {
            fprintf(stderr, "Usage: %s add <file> <name>\n", argv[0]);
            return 1;
        }
        add_sound_to_database(argv[2], argv[3]);
    } else if (strcmp(argv[1], "match") == 0) {
        if (argc < 3) {
            fprintf(stderr, "Usage: %s match <file>\n", argv[0]);
            return 1;
        }
        match_sound(argv[2]);
    } else if (strcmp(argv[1], "list") == 0) {
        list_database();
    } else {
        fprintf(stderr, "Unknown command: %s\n", argv[1]);
        return 1;
    }

    return 0;
}
