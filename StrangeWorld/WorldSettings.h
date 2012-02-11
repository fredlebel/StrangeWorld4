//#define USE_ELITISM     // Merge with elite gene on spawning
#define ELITE_SAMPLING  30 // Pool size for sampling elitism

#define DECAY_RATE      1       // How much energy a corpse loses per tick.
#define SPAWN_HURT      300     // Health lost when spawning
#define SPAWN_HEALTH    500     // Initial health for new creatures
#define GRASS_HEAL      1       // Health grass gains per tick

#define MAX_EYE_RADIUS          150.0
#define MIN_EYE_RADIUS           10.0
#define EYE_RADIUS_MULTIPLYER     6.0     // 6 * 150 = 900, at close to 1000 as possible
#define MAX_EYE_ANGLE            45.0
#define MIN_EYE_ANGLE             0.0
#define EYE_ANGLE_MULTIPLYER     10.0     // 10 * 90 = 900, at close to 1000 as possible
#define FEED_AMOUNT              20.0     // How much energy is taken when feeding.
#define MIN_BODY_RADIUS           2.0
#define MAX_BODY_RADIUS          10.0

#define CARNIVORE_OFFSPRING_COUNT    1      // Number of offsprings when spawing
#define CARNIVORE_TICK_HURT          4      // Ticks before losing one energy
#define CARNIVORE_SPAWN_THRES      900      // Required health to spawn
#define CARNIVORE_SPAWN_AGE_THRES  750      // How old before breeding

#define HERBIVORE_OFFSPRING_COUNT    1      // Number of offsprings when spawing
#define HERBIVORE_TICK_HURT         10      // Ticks before losing one energy
#define HERBIVORE_SPAWN_THRES      800      // Required health to spawn
#define HERBIVORE_SPAWN_AGE_THRES  750      // How old before breeding

//#define USE_STATIC_NEURALNET    // Don't add/remove neurons

#define MUTATION_BIAS_SOFT         200 // Small changes to the bias
// Bigger numbers indicate rarer mutations.
//#define MUTATION_BIAS_INVERSE    10000 // A dendrite value is inversed
#define MUTATION_BIAS_ADDREM      2500 // A dendrite is added/removed
//#define MUTATION_BIAS_CHANGE     20000 // A dendrite value is randomized
//#define MUTATION_BIAS_MOVE       20000 // A dendrite is reconnected to another neuron.
#define MUTATION_NEURON_ADDREM   60000 // A neuron is added/removed


#define GENEFILE_HEADER          "SW5 DNA FREE"
#define GENEFILE_TOKEN_NEURON    "@"
#define GENEFILE_TOKEN_DENDRITE  "!"
#define GENEFILE_TOKEN_COMMA     ","

// Used only when repopulating due to extinctions.
#define ELITE_IS_OLDEST
#define ELITE_IS_PROLIFIC
#define ELITE_EATS_MOST

