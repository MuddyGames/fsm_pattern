# Practical 9: The Finite State Machine (FSM) Pattern

In this practical, the player and an NPC each get a **finite state machine**. At any moment each one is in exactly one **state** (Idle, Moving, Firing, Jumping, Dead or Respawn), and it only changes state when an **event** arrives and its **transition table** allows the move. It's the last two practicals put together: the **Command** and **Mediator** patterns handle input, **cute_c2** handles collisions, and the FSM decides what each object does about it all.

Your job is to fix a transition that's missing, make the player and NPC die and come back, add states and commands of your own, and give the NPC a bit of a brain.

By the end you'll understand why games use state machines instead of a pile of `bool isJumping, isFiring, isDead` flags, and how function pointers let each state bring its own behaviour.

> The project was generated with [raylib project creator](https://raysan5.itch.io/raylib-project-creator) by raysan5.

---

## Short version

If you just want to get going, this is the whole thing in a few lines:

**1. Clone**

```bash
mkdir -p ~/Projects
cd ~/Projects
git clone https://github.com/MuddyGames/fsm_pattern
```

**2. Set up raylib**

*Linux:*

```bash
cd ~/Projects/fsm_pattern/setup
bash setup_linux.sh
```

or make the script executable and run it directly:

```bash
cd ~/Projects/fsm_pattern/setup
chmod +x setup_linux.sh
./setup_linux.sh
```

*Windows* (install [raylib](https://raysan5.itch.io/raylib) first, and then use w64devkit terminal):

```bash
cd ~/Projects/fsm_pattern/setup
sh setup_windows.sh
```

**3. Build and run**

```bash
cd ~/Projects/fsm_pattern
make run                            # desktop
```

The rest of this page explains each step, and what to do when the computer says no.

---

## Step 1: Get the project

Keep all your work in a `Projects` folder in your home directory.

- **Linux:** open a terminal.
- **Windows:** open the **w64devkit** terminal (`C:\raylib\w64devkit\w64devkit.exe`).

Then type the same commands on both:

```bash
mkdir -p ~/Projects
cd ~/Projects
git clone https://github.com/MuddyGames/fsm_pattern
```

Your project now lives in `~/Projects/fsm_pattern`.

---

## Step 2: Install raylib and Emscripten (once only)

If you set these up for an earlier practical, skip this step. They're shared by every raylib project.

### Linux (Debian, Ubuntu, Pop!_OS or Arch)

```bash
cd ~/Projects/fsm_pattern/setup
bash setup_linux.sh
```

It will ask for your password to install packages. It takes a few minutes, especially on lab machines.

### Windows

Install the **[raylib installer](https://raysan5.itch.io/raylib)** and keep the default location, `C:\raylib`. Then in the **w64devkit** terminal:

```bash
cd ~/Projects/fsm_pattern/setup
sh setup_windows.sh
```

When it finishes, close w64devkit and open it again.

> No more MSYS2. Everything now runs in the w64devkit terminal that comes with the raylib installer.

For the manual steps, see the [drawing_primitives README](https://github.com/MuddyGames/drawing_primitives#manual-setup-only-if-the-script-didnt-work-for-you).

---

## Step 3: Build and run

Every command runs from the project folder:

```bash
cd ~/Projects/fsm_pattern
```

### On your desktop

The same commands work on Linux and Windows (w64devkit):

| Command | What it does |
|---|---|
| `make` | Builds the game |
| `make run` | Builds it and runs it |
| `make BUILD_MODE=DEBUG` | Builds with debug info, for stepping through in the debugger |
| `make clean` | Deletes the `build` folder, so the next build starts fresh |

A window should open with two circles: the **player** (green, in the middle) and an **NPC guard** (grey, top right). The HUD in the top left shows the command bits and the player's current state.

| Key | Action |
|---|---|
| WASD or arrow keys, D-pad | Move |
| SPACE, or the bottom face button | Jump |
| Left mouse button, or the right trigger | Fire |

Bump into the NPC and both health bars drop.

Keep the terminal where you can see it. Every state change is printed there, and it's the best way to watch the FSM working.

### In your browser

```bash
make run_web
```

This builds the web version, starts a small local web server and opens
**<http://127.0.0.1:8000/fsm_pattern.html>** in your browser.

Press **Ctrl+C** in the terminal to stop the server.

| Command | What it does |
|---|---|
| `make web` | Builds the web version only |
| `make run_web` | Builds it and opens it in your browser |
| `make stop_web` | Stops a web server left running from earlier |

In the browser, the state changes appear in the developer console (**F12**).

### In VS Code

Open `projects/VSCode/main.code-workspace` (**File → Open Workspace from File…**).

- **Ctrl+Shift+B** builds.
- **F5** builds and starts the debugger. The first time, choose **Debug** in the Run and Debug drop-down.
- **Terminal → Run Task** lists Run, Run Web, Stop Web Server and Clean.

Set a breakpoint in `ChangeState()` in `fsm.c` and press SPACE. Look at `obj->currentState` and `newState`, then step into `currentConfig->Exit` and watch it land in the right state's function. That's function pointers doing their job.

---

## How the project is laid out

```
fsm_pattern/
├── Makefile               build instructions
├── src/
│   ├── main.c             opens the window, runs the game loop
│   ├── game.c             sets up, updates and draws the game; checks collisions
│   ├── fsm.c              the state machine: HandleEvent, ChangeState, UpdateState
│   ├── gameobject.c       what the player and NPC have in common
│   ├── player.c           the player's states and transition tables
│   ├── npc.c              the NPC's states and transition tables
│   ├── input_manager.c    turns key, mouse and button presses into commands
│   ├── ai_manager.c       makes up commands for the NPC
│   ├── mediator.c         passes each command to a game object and the HUD
│   ├── command.c          ExecuteCommand(): turns command bits into FSM events
│   ├── hud.c              shows the command bits and the player's state on screen
│   ├── collision.c        collision checks, pushback and damage
│   └── cute_c2_impl.c     builds the cute_c2 collision library; leave it alone
├── include/
│   ├── fsm.h              the State enum and the StateConfig struct
│   ├── events.h           the Event enum
│   ├── command.h          the command bit flags
│   ├── gameobject.h       the GameObject struct
│   ├── player.h, npc.h    Player and NPC structs, and their state functions
│   ├── hud.h              Hud struct
│   ├── constants.h        screen size, damage and other constants
│   ├── cute_c2.h          the cute_c2 collision library
│   └── …                  one header for each .c file
├── resources/             sprites and images
├── build/                 everything make creates
├── platform/              icons and web page template; leave these alone
└── setup/                 setup scripts (run once)
```

Each frame, `UpdateGame()` in `game.c` does the same three things for the player:

```c
data->activeCommand = PollInput();                                             // 1. keys -> command bits
MediatorHandleCommand(data->playerMediator, data->activeCommand, deltaTime);   // 2. command bits -> events -> maybe a new state
UpdateState((GameObject *)data->player, deltaTime);                            // 3. run the current state's Update
```

The mediator passes the command to its two **colleagues**, just like the last practical:

```c
void MediatorHandleCommand(Mediator *mediator, Command command, float deltaTime)
{
    ExecuteCommand(command, mediator->object, deltaTime);   // 1. the game object acts on it (through its FSM)

    if (mediator->hud)                                      // 2. the HUD shows it, with the state's name
    {
        GameObject *object = mediator->object;
        UpdateHud(mediator->hud, command, object->stateConfigs[object->currentState].name);
    }
}
```

The NPC does exactly the same, except its commands come from `PollAI()` instead of the keyboard, and its mediator has no HUD (`hud` is `NULL`). The FSM can't tell the difference.

**What changed from the command pattern?** Only `ExecuteCommand()`. Before, it called the player's actions directly (`MoveLeft()`, `Jump()`). Now it sends **events** (`EVENT_MOVE`, `EVENT_JUMP`), and the **current state** decides what they mean.

---

## How it all fits together

[![](https://mermaid.ink/img/pako:eNp1U1tvmzAU_isWk_qUJqu0PrSbKnENbCWJStIXqCIHDok3YyJjsqKq_33GNrm0GgjJPt_lfObIb1ZeF2DdI2vL8X6Hlk7GkHzsNGL7ViAPDiSH5seGTx5-QbepMS8mbs0ErykF_oKurx-QY8gxZngLXJEXElfVF2MYpXb0iWFH2sHVJEdvUreuKswK5BDxlzSg6PH82V-vFujqAFx8R0H05BtrV6m8NIaCYFFr92ETSh8KxtAIPCXwj238V8hbQWqmlHr3WTJGe4o74KhmtENj6RCuvFR-SrXaF1hA2A4CX_ZA_rM_W6775COz_rmKFyM0Ho9VhCANkljJdUz_AEygCXJ3mG0hEdLR2AWKP02nuIL55jfkQv9DlWgyW7g6upxLJ_U6jFz4r2QYwFQ5hPLQlJJGHhYlXSOgUkJ3B_mfI2IU4ekI7vzxMUqi-WydLO2nZe88038xyJhmN6KjgGxUEkrvv8BNeVuWF0j0X8gxSFmW3-DmHHGPSK86R7wBuZXIhZt_6lNeuslZDaqv_XuOBWedLlXTU7q7D36nTnd9BmuErAp4hUnR36i3nptZYgcVZLKQWQWUuKUiszL23pNxK-qkY7kEBW9BVlo1OY9geR0rU37_B-A1Iw4?type=png)](https://mermaid.live/edit#pako:eNp1U1tvmzAU_isWk_qUJqu0PrSbKnENbCWJStIXqCIHDok3YyJjsqKq_33GNrm0GgjJPt_lfObIb1ZeF2DdI2vL8X6Hlk7GkHzsNGL7ViAPDiSH5seGTx5-QbepMS8mbs0ErykF_oKurx-QY8gxZngLXJEXElfVF2MYpXb0iWFH2sHVJEdvUreuKswK5BDxlzSg6PH82V-vFujqAFx8R0H05BtrV6m8NIaCYFFr92ETSh8KxtAIPCXwj238V8hbQWqmlHr3WTJGe4o74KhmtENj6RCuvFR-SrXaF1hA2A4CX_ZA_rM_W6775COz_rmKFyM0Ho9VhCANkljJdUz_AEygCXJ3mG0hEdLR2AWKP02nuIL55jfkQv9DlWgyW7g6upxLJ_U6jFz4r2QYwFQ5hPLQlJJGHhYlXSOgUkJ3B_mfI2IU4ekI7vzxMUqi-WydLO2nZe88038xyJhmN6KjgGxUEkrvv8BNeVuWF0j0X8gxSFmW3-DmHHGPSK86R7wBuZXIhZt_6lNeuslZDaqv_XuOBWedLlXTU7q7D36nTnd9BmuErAp4hUnR36i3nptZYgcVZLKQWQWUuKUiszL23pNxK-qkY7kEBW9BVlo1OY9geR0rU37_B-A1Iw4)

Four systems work together:

1. **Finite State Machine (FSM):** keeps track of what each game object is doing (Idle, Moving, Firing, Jumping, Dead, Respawn). Events cause state changes, and each state has its own Entry, Update and Exit functions.
2. **Command system:** each input is a bit flag, so several commands can happen at once (e.g. `MOVE_LEFT | JUMP`). Input knows nothing about game logic.
3. **Collision detection:** circle-to-circle collisions with cute_c2, using a manifold to push objects apart. Collisions send events too (`EVENT_COLLISION_START`, `EVENT_COLLISION_END`).
4. **Mediator:** passes each command to the game object and the HUD, so they never need to know about each other.

---

## The state machine

### States

- `STATE_IDLE`: at rest
- `STATE_MOVING`: in motion
- `STATE_FIRING`: attacking
- `STATE_JUMPING`: jumping
- `STATE_DEAD`: defeated
- `STATE_RESPAWN`: coming back

[![](https://mermaid.ink/img/pako:eNqdk2tLwzAUhv9KOR-lG70n5oMwbJWK68Y2J2hFwppdYE1Glopz9L_bdrfOYRHzKe95n_MmB5ItTETCgMBaUcX8BZ1JmrY-rJhrxXq9etNarRttOOqMgvfQfwx29ZOu2d3eOIzuNaIF4yAalfKM3tvncUc66kXN9F04qKcX8ozf239M_3Hzh6duvx5e6jp-8P-X7gcd_8j6YfOYTfDFjE3w5Z2b6Mr7ZbxBMOx3nqOYlw2gQ8pkShdJ8Wa2ZSUGNWcpi4EU24RNabZUMcQ8L1CaKTHc8AkQJTOmgxTZbA5kSpfrQmWr5PTmjtUV5S9CpIeWmSyP2rcznjB5KzKugJgurmAgW_gEYtttD7nINZBtOZaJkavDpqRQ28TX2HAMhEwHO06uw1eVb7QdhD3DdF3PNC2MLNvTgSULJWR39yeqr5F_A_5z4Kw?type=png)](https://mermaid.live/edit#pako:eNqdk2tLwzAUhv9KOR-lG70n5oMwbJWK68Y2J2hFwppdYE1Glopz9L_bdrfOYRHzKe95n_MmB5ItTETCgMBaUcX8BZ1JmrY-rJhrxXq9etNarRttOOqMgvfQfwx29ZOu2d3eOIzuNaIF4yAalfKM3tvncUc66kXN9F04qKcX8ozf239M_3Hzh6duvx5e6jp-8P-X7gcd_8j6YfOYTfDFjE3w5Z2b6Mr7ZbxBMOx3nqOYlw2gQ8pkShdJ8Wa2ZSUGNWcpi4EU24RNabZUMcQ8L1CaKTHc8AkQJTOmgxTZbA5kSpfrQmWr5PTmjtUV5S9CpIeWmSyP2rcznjB5KzKugJgurmAgW_gEYtttD7nINZBtOZaJkavDpqRQ28TX2HAMhEwHO06uw1eVb7QdhD3DdF3PNC2MLNvTgSULJWR39yeqr5F_A_5z4Kw)

The object is always in exactly **one** state, stored in `currentState`. The one before it is kept in `previousState`.

### State configuration

Every state is described by a `StateConfig`: a name, four function pointers and its transition table.

```c
typedef struct StateConfig
{
    const char *name;          // e.g. "Player_Idle"
    EventFunction HandleEvent; // what this state does when an event arrives
    StateFunction Entry;       // runs once, when the state starts
    StateFunction Update;      // runs every frame while in the state
    StateFunction Exit;        // runs once, when the state ends
    State *nextStates;         // the states it may change to
    int nextStatesCount;
} StateConfig;
```

- **HandleEvent:** decides what to do with each event, usually by asking for a new state with `ChangeState()`.
- **Entry:** called **once**, when the state starts. Reset timers, set the colour.
- **Update:** called **every frame** while in the state. Movement and timers go here.
- **Exit:** called **once**, when the state ends. Tidy up.

### Example state setup

The player's Idle state is set up like this in `InitPlayerFSM()`:

```c
// Define valid transitions from IDLE
State idleValidTransitions[] = {STATE_MOVING, STATE_FIRING, STATE_JUMPING, STATE_DEAD};

// Configure IDLE state
object->stateConfigs[STATE_IDLE].name = "Player_Idle";
object->stateConfigs[STATE_IDLE].HandleEvent = PlayerIdleHandleEvent;
object->stateConfigs[STATE_IDLE].Entry = PlayerEnterIdle;
object->stateConfigs[STATE_IDLE].Update = PlayerUpdateIdle;
object->stateConfigs[STATE_IDLE].Exit = PlayerExitIdle;

// Set valid transitions
StateTransitions(&object->stateConfigs[STATE_IDLE], idleValidTransitions,
                 sizeof(idleValidTransitions) / sizeof(State));
```

When a state's `HandleEvent` asks for a new state, `ChangeState()` checks the table first. If the move is allowed, it runs the old state's `Exit()`, then the new state's `Entry()`. If not, it prints `Invalid state transition` and nothing changes.

### The player's state diagram

[![](https://mermaid.ink/img/pako:eNqNk1FrwjAUhf9KuE_bqNIW22oeBrJ2wzGraOdg6xjBRg3YVGK66cT_vtTq7Cy0y1NO8p17Twh3B9MkooBhLYmkLiNzQeLGpxlypNbbzTtqNG7ROOgG3kfPffLy87MuXPcHk57_gJE38fwgU1XwfW9UgJWqgh-f-8MCnckK2vW67gl1e8e6RT4PevGwk8Mf-F4lXJ28hNdkL_G16fP-pfQBi6lA3mbFBF1X8qd_-uNAV19MLhDjq1ReV9prAx5f_P-EZUNtjwwo8CNvPOy--Bh1U5mgQBC-ZpIlvGg5MqVYFxbQQMWMCYvUUOyyAiHIBY1pCFhtIzoj6VKGEPK9Qokyj7d8CliKlGogknS-ADwjy7VS6So6D9Xv6Yrw1ySJT5a5yFod7ZRHVNwlKZeALfPAAt7BBrBtN9uObRm6beqG4RgdDbaATaup222r1TZ10zJatrXX4PtQXFe4Y7dbRqejm4bT6jga0IjJRPTzgT_M_f4Hvy8ksA?type=png)](https://mermaid.live/edit#pako:eNqNk1FrwjAUhf9KuE_bqNIW22oeBrJ2wzGraOdg6xjBRg3YVGK66cT_vtTq7Cy0y1NO8p17Twh3B9MkooBhLYmkLiNzQeLGpxlypNbbzTtqNG7ROOgG3kfPffLy87MuXPcHk57_gJE38fwgU1XwfW9UgJWqgh-f-8MCnckK2vW67gl1e8e6RT4PevGwk8Mf-F4lXJ28hNdkL_G16fP-pfQBi6lA3mbFBF1X8qd_-uNAV19MLhDjq1ReV9prAx5f_P-EZUNtjwwo8CNvPOy--Bh1U5mgQBC-ZpIlvGg5MqVYFxbQQMWMCYvUUOyyAiHIBY1pCFhtIzoj6VKGEPK9Qokyj7d8CliKlGogknS-ADwjy7VS6So6D9Xv6Yrw1ySJT5a5yFod7ZRHVNwlKZeALfPAAt7BBrBtN9uObRm6beqG4RgdDbaATaup222r1TZ10zJatrXX4PtQXFe4Y7dbRqejm4bT6jga0IjJRPTzgT_M_f4Hvy8ksA)

This is how the player is **meant** to work. Compare it with the transition tables in `player.c`: one arrow is missing from the code. You'll fix it in Task 2.

> **Why not just use `bool`s?** With `isJumping`, `isFiring` and `isDead`, nothing stops all three being `true` at once, and every new flag doubles the number of combinations to check. An FSM is in exactly one state, and the transition tables say, in one place, what can follow what.

---

## Events

Events are what happened. They're defined in `events.h`, and they cause state changes:

```c
typedef enum
{
    EVENT_NONE,             // No action
    EVENT_MOVE,             // Movement input
    EVENT_FIRE,             // Attack action
    EVENT_JUMP,             // Jump action
    EVENT_DIE,              // Death condition
    EVENT_RESPAWN,          // Respawn trigger
    EVENT_COLLISION_START,  // Collision begins
    EVENT_COLLISION_END,    // Collision ends
    EVENT_COUNT             // Total event count
} Event;
```

The same event can mean different things in different states. `EVENT_FIRE` starts firing in Idle, but Jumping ignores it.

---

## Commands as bit flags

The command system is the one from the last practical. Every command gets its own bit, so one number can hold any mix of them:

```c
typedef enum
{
    NONE          = 0,        // No command
    MOVE_UP       = 1 << 0,   // Binary: 000001
    MOVE_DOWN     = 1 << 1,   // Binary: 000010
    MOVE_LEFT     = 1 << 2,   // Binary: 000100
    MOVE_RIGHT    = 1 << 3,   // Binary: 001000
    JUMP          = 1 << 4,   // Binary: 010000
    FIRE          = 1 << 5,   // Binary: 100000
    COMMAND_COUNT = 6         // How many commands there are: NOT a flag
} Command;
```

> **`COMMAND_COUNT` is a number, not a flag.** Never OR it into a command. When you add a command, increase it by one.

### Combining commands

```c
// Move left AND jump simultaneously
Command combo = MOVE_LEFT | JUMP;
// Binary: 000100 | 010000 = 010100

if (IsCommandActive(combo, MOVE_LEFT))   // TRUE: bit 2 is set
    // ...

if (IsCommandActive(combo, JUMP))        // TRUE: bit 4 is set
    // ...
```

### From command to event

[![](https://mermaid.ink/img/pako:eNpVkm9r2zAQxr-KUKFv6mZW_KeOMwqJk42VhYaWdjC7DMU-Jd4sychSaRby3acoTpfei0O65_ndCXQ7XMoKcIrXirYb9P2hEMjGJF82dAsKfROt0Z9X6tPtj6vHlpaAWgVdB9ULur6-RdN8KZvGmdBF6I9tImNnfwBtlOhQJjmnono59p06Ksv7KvrS0HXn_Iv75_mvpyW6fAWlx-juabF0dZ_4vk96PHP4LJ-_QWk0nLp8nJxtoPyDaKnrV0CrWnc9PHPwPP9KOdyvfkOp0cSapOgfIK39Ct0Z3vbAMXd62wCaIFY3TXoBhEWMnSvTXmGMhUDOlexdOVDnyux_N_aRmZ-Y6KBhD3NQnNaV_aHdwVdgvQEOBU7tsQJGTaMLXIi9tVKj5eNWlDjVyoCHlTTrDU4ZbTp7M21FNcxqan-av1dbKn5KyU_IWh1G9TiIClQmjdA4jWLnxekOv-E0iQbRcJTEJIx8MooC4uEtTsNgQOKb-MbWkoD4UbT38F_X3B8kJIjJMAlGw9BGHHgYqlpLtThun1vC_T_t4cec?type=png)](https://mermaid.live/edit#pako:eNpVkm9r2zAQxr-KUKFv6mZW_KeOMwqJk42VhYaWdjC7DMU-Jd4sychSaRby3acoTpfei0O65_ndCXQ7XMoKcIrXirYb9P2hEMjGJF82dAsKfROt0Z9X6tPtj6vHlpaAWgVdB9ULur6-RdN8KZvGmdBF6I9tImNnfwBtlOhQJjmnono59p06Ksv7KvrS0HXn_Iv75_mvpyW6fAWlx-juabF0dZ_4vk96PHP4LJ-_QWk0nLp8nJxtoPyDaKnrV0CrWnc9PHPwPP9KOdyvfkOp0cSapOgfIK39Ct0Z3vbAMXd62wCaIFY3TXoBhEWMnSvTXmGMhUDOlexdOVDnyux_N_aRmZ-Y6KBhD3NQnNaV_aHdwVdgvQEOBU7tsQJGTaMLXIi9tVKj5eNWlDjVyoCHlTTrDU4ZbTp7M21FNcxqan-av1dbKn5KyU_IWh1G9TiIClQmjdA4jWLnxekOv-E0iQbRcJTEJIx8MooC4uEtTsNgQOKb-MbWkoD4UbT38F_X3B8kJIjJMAlGw9BGHHgYqlpLtThun1vC_T_t4cec)

`ExecuteCommand()` in `command.c` turns the bits into events:

- any movement bit sets `inputAxis` and sends `EVENT_MOVE`; no movement sends `EVENT_NONE`,
- `JUMP` sends `EVENT_JUMP`,
- `FIRE` sends `EVENT_FIRE`.

Several events can arrive in the same frame, one after the other.

---

## Collision detection

The collision system uses cute_c2 circles. `UpdateGame()` tracks when a collision starts, continues and ends:

```c
// Update colliders to match the objects
data->player->base.collider.p.x = data->player->base.x;
data->player->base.collider.p.y = data->player->base.y;
data->player->base.collider.r = data->player->base.r;

// Check collision
bool isColliding = CheckCollision(&data->player->base, &data->npc->base);

if (isColliding && !data->player->base.isColliding)
{
    // ENTER: first frame of the collision
    HandleEvent(&data->player->base, EVENT_COLLISION_START, deltaTime);
    CollisionEntry(&data->player->base, &data->npc->base);   // damage
    HandleCollision(&data->player->base, &data->npc->base);  // pushback
}
else if (!isColliding && data->player->base.isColliding)
{
    // EXIT: first frame after the collision ends
    HandleEvent(&data->player->base, EVENT_COLLISION_END, deltaTime);
    CollisionExit(&data->player->base, &data->npc->base);
}
else if (isColliding && data->player->base.isColliding)
{
    // ONGOING: keep pushing apart
    HandleCollision(&data->player->base, &data->npc->base);
}
```

The NPC gets the same events. Damage happens once, on ENTER, so the health bars don't drain 60 times a second.

---

## Game objects

The player and NPC are both a `GameObject`, with extras:

[![](https://mermaid.ink/img/pako:eNqtVe1u2jAUfRXLUiW6pqihUEJUIVUQOqbxoUI7aWOqXHJJUiU2cpy2rOLdZydpMRjUbVp-JPa5x_Y5vtfxK54zH7CL5zFJ025EAk6SGUXyOTpCHs2StOjlceQ9ARXotYDUc3kJktNub5AT784bTu-Ho6FngIPRnQn2-jcm-OV2MDbAbt8k3niT8dW3oYF3RrfDaYGuZ1T3MBFEwAceJtOrqXff7371DFB66A-vDVi62AcrH_vwrnfVNcB9mmUWrkkCo4dHmAsLDccdCxHqo3FMVsALS7CVow1dN3kyETyiAaIyuoWqzVhyeIpYluY9IzrPOJdp3xvsMLqIgh8_UbrppRopkuUSAolFqIFPLPJRn0ZiI7WidB3vcroQgwCNZTA-y72IIS_LSlGcoN4674GxGHUI9agAnouuFL4oPOcNkxwSGsBH1Hz926Uv4YJ6fLjcio35g3zkHnoZnYuIUd3d7sa_c6QtvjoYLfQdHvwSGTPLZFJ4KdK9m8pNoMOyN1E7hmWJbhlV40gQcEhTuaQWUERVBfK7P_1lsDcZVLSiZg-Pf1kGOwLLo6NrXMSMCFXDSUSJgSdkCyzHK3VF87D6Iv7_DMi_wQ3EROUuDaNlmR61kaenbf3kuyiiIfBIlJRS8wcsLTTD9gznfNmqVj_Jjl7JLgpJqqkaczaXCUa9mD0bc1Wr7fLmcOWvJidCupdVnDYXZXnZavZUcEt4JqfQdw0J9jY30g5M6ftfhmMLJ8ATEvnyesyLZYZFCPK4Ylc2fViQLBYzPKNrSSWZYJMVnWNX8AwszFkWhNhdkDiVvcJPeb2-o0tCvzOWvA0JuFqqHA7UB54fMuzWGrWcjN1X_IJdu1av1pp2_cw5c-yzRv2iZeEVds-bVcexnXrz3LEbLdtZW_hXPrtdtR2n0Wi2WheS3mzaTQuDHwnGB-Xdrz7r39PqaTE?type=png)](https://mermaid.live/edit#pako:eNqtVe1u2jAUfRXLUiW6pqihUEJUIVUQOqbxoUI7aWOqXHJJUiU2cpy2rOLdZydpMRjUbVp-JPa5x_Y5vtfxK54zH7CL5zFJ025EAk6SGUXyOTpCHs2StOjlceQ9ARXotYDUc3kJktNub5AT784bTu-Ho6FngIPRnQn2-jcm-OV2MDbAbt8k3niT8dW3oYF3RrfDaYGuZ1T3MBFEwAceJtOrqXff7371DFB66A-vDVi62AcrH_vwrnfVNcB9mmUWrkkCo4dHmAsLDccdCxHqo3FMVsALS7CVow1dN3kyETyiAaIyuoWqzVhyeIpYluY9IzrPOJdp3xvsMLqIgh8_UbrppRopkuUSAolFqIFPLPJRn0ZiI7WidB3vcroQgwCNZTA-y72IIS_LSlGcoN4674GxGHUI9agAnouuFL4oPOcNkxwSGsBH1Hz926Uv4YJ6fLjcio35g3zkHnoZnYuIUd3d7sa_c6QtvjoYLfQdHvwSGTPLZFJ4KdK9m8pNoMOyN1E7hmWJbhlV40gQcEhTuaQWUERVBfK7P_1lsDcZVLSiZg-Pf1kGOwLLo6NrXMSMCFXDSUSJgSdkCyzHK3VF87D6Iv7_DMi_wQ3EROUuDaNlmR61kaenbf3kuyiiIfBIlJRS8wcsLTTD9gznfNmqVj_Jjl7JLgpJqqkaczaXCUa9mD0bc1Wr7fLmcOWvJidCupdVnDYXZXnZavZUcEt4JqfQdw0J9jY30g5M6ftfhmMLJ8ATEvnyesyLZYZFCPK4Ylc2fViQLBYzPKNrSSWZYJMVnWNX8AwszFkWhNhdkDiVvcJPeb2-o0tCvzOWvA0JuFqqHA7UB54fMuzWGrWcjN1X_IJdu1av1pp2_cw5c-yzRv2iZeEVds-bVcexnXrz3LEbLdtZW_hXPrtdtR2n0Wi2WheS3mzaTQuDHwnGB-Xdrz7r39PqaTE)

The `GameObject` struct, grouped by what each field is for:

```c
typedef struct GameObject
{
    const char *name;          // Object identifier

    // Transform
    float x, y;                // Position
    int r;                     // Radius
    Vector2 direction;         // Movement direction
    Vector2 inputAxis;         // Input direction

    // Collision
    c2Circle collider;         // Circle collider
    c2Manifold manifold;       // Collision manifold
    bool isColliding;          // Collision state

    // Visual
    Color color;               // Render color

    // FSM
    State previousState;       // Previous state
    State currentState;        // Current state
    StateConfig *stateConfigs; // State configurations

    // Gameplay
    int health;                // Health points
    float timer;               // General-purpose timer
} GameObject;
```

`Player` adds `stamina` and `mana`, and `NPC` adds `aggression`. Neither is used much yet; that's up to you. Because `base` is the **first** member of both, a `Player *` can be cast to a `GameObject *` and back:

```c
Player *player = (Player *)object;   // inside a state function
player->stamina -= 20.0f;
```

Both use the same FSM code in `fsm.c`, but each has its own states, transition tables and functions in `player.c` and `npc.c`. The NPC only uses Idle, Firing and Dead. Its other states are set to `EMPTY_STATE_CONFIG`.

---

## Walkthrough: adding Crouch to the FSM

Follow these steps to add a new state, **`STATE_CROUCH`**, to the player. Hold **C** to crouch; let go to stand up. Every new state follows the same pattern, so it's worth doing this one carefully before the tasks.

### 1. Add `CROUCH` to `Command`

In **`command.h`**, give it the next free bit, and update `COMMAND_COUNT`:

```c
	FIRE 			= 1 << 5,	// Binary: 100000
	CROUCH 			= 1 << 6,	// Binary: 1000000
	COMMAND_COUNT	= 7 // Total number of commands
```

### 2. Add `EVENT_CROUCH` to `Event`

In **`events.h`**, add it **before** `EVENT_COUNT`:

```c
    EVENT_COLLISION_END,   // Collision end event
    EVENT_CROUCH,          // Crouch event
    EVENT_COUNT            // Total number of events (for counting purposes)
```

### 3. Add `STATE_CROUCH` to the FSM

In **`fsm.h`**, add it **before** `STATE_COUNT`:

```c
    STATE_RESPAWN, // Respawn state (respawns the player)
    STATE_CROUCH,  // Crouch state
    STATE_COUNT    // Total number of states (for counting purposes)
```

### 4. Write the Crouch functions

These are the four function pointers in `StateConfig`. Declare them in **`player.h`**:

```c
// Crouch state
void PlayerCrouchHandleEvent(GameObject *object, Event event, float deltaTime);
void PlayerEnterCrouch(GameObject *object, float deltaTime);
void PlayerUpdateCrouch(GameObject *object, float deltaTime);
void PlayerExitCrouch(GameObject *object, float deltaTime);
```

and write them in **`player.c`**:

```c
// Handles events for the Player when in the Crouch state
void PlayerCrouchHandleEvent(GameObject *object, Event event, float deltaTime)
{
	switch (event)
	{
	case EVENT_NONE:
		// C released, no movement: stand up
		ChangeState(object, STATE_IDLE, deltaTime);
		break;
	// Ignore Events for other cases
	case EVENT_MOVE:
	case EVENT_FIRE:
	case EVENT_JUMP:
	case EVENT_DIE:
	case EVENT_RESPAWN:
	case EVENT_COLLISION_START:
	case EVENT_COLLISION_END:
	case EVENT_CROUCH:
	case EVENT_COUNT:
		break;
	}
}

void PlayerEnterCrouch(GameObject *object, float deltaTime)
{
	(void)deltaTime;
	printf("\n%s -> ENTER -> Crouch\n", object->name);
	object->color = DARKBLUE;
	object->r = DEFAULT_GAMEOBJECT_RADIUS / 2; // smaller while crouching
}

void PlayerUpdateCrouch(GameObject *object, float deltaTime)
{
	(void)object;
	(void)deltaTime;
	// Nothing to do while crouching (yet)
}

void PlayerExitCrouch(GameObject *object, float deltaTime)
{
	(void)deltaTime;
	printf("\n%s <- EXIT <- Crouch\n", object->name);
	object->r = DEFAULT_GAMEOBJECT_RADIUS;
}
```

### 5. Add the state configuration

At the end of `InitPlayerFSM()` in **`player.c`**, register the functions and the transition table:

```c
	// ---- STATE_CROUCH state configuration ----
	State crouchValidTransitions[] = {STATE_IDLE};

	object->stateConfigs[STATE_CROUCH].name = "Player_Crouch";
	object->stateConfigs[STATE_CROUCH].HandleEvent = PlayerCrouchHandleEvent;
	object->stateConfigs[STATE_CROUCH].Entry = PlayerEnterCrouch;
	object->stateConfigs[STATE_CROUCH].Update = PlayerUpdateCrouch;
	object->stateConfigs[STATE_CROUCH].Exit = PlayerExitCrouch;

	StateTransitions(&object->stateConfigs[STATE_CROUCH], crouchValidTransitions, sizeof(crouchValidTransitions) / sizeof(State));
```

### 6. Define how to enter and exit Crouch

[![](https://mermaid.ink/img/pako:eNp1UVtrwjAU_ivl7EEYtTRNrW0cwqgFha3CdHvYOkZmjhdmE0lTmRP_--JlUwYLBJLvfJecnC1MlEBgUBlusLfgM83L5joopGPXy_Wr02x2ndH4dpy9DXp3WSGPlTNyQUgfho9pnzmNtOF84MZZaawqFDfvutvnUixxYHe2RmmuQr_jeVch6eyL2VOWj0_qS_sj8ucF1j5Xl_aNs3-qVT2Z_5uQD3PbALhQoi75Qti2t_u8AswcSyyA2aPAKa-XpoBC7iyV10aNNnICzOgaXbAJszmwKV9W9lavxPnbftEVl89KlT-Smd5HneQoBepU1dIAozQ6kIFt4RNYkCSeT0jcJpS248S3xQ0wCyZRlNAw8EPik1a0c-Hr4O57rYC2w5iGhIZxHNGWCygWRun740wPo919A_5pkcg?type=png)](https://mermaid.live/edit#pako:eNp1UVtrwjAU_ivl7EEYtTRNrW0cwqgFha3CdHvYOkZmjhdmE0lTmRP_--JlUwYLBJLvfJecnC1MlEBgUBlusLfgM83L5joopGPXy_Wr02x2ndH4dpy9DXp3WSGPlTNyQUgfho9pnzmNtOF84MZZaawqFDfvutvnUixxYHe2RmmuQr_jeVch6eyL2VOWj0_qS_sj8ucF1j5Xl_aNs3-qVT2Z_5uQD3PbALhQoi75Qti2t_u8AswcSyyA2aPAKa-XpoBC7iyV10aNNnICzOgaXbAJszmwKV9W9lavxPnbftEVl89KlT-Smd5HneQoBepU1dIAozQ6kIFt4RNYkCSeT0jcJpS248S3xQ0wCyZRlNAw8EPik1a0c-Hr4O57rYC2w5iGhIZxHNGWCygWRun740wPo919A_5pkcg)

**Entering Crouch** happens in Idle. Add `STATE_CROUCH` to Idle's transition table:

```c
State idleValidTransitions[] = {STATE_MOVING, STATE_FIRING, STATE_JUMPING, STATE_DEAD, STATE_CROUCH};
```

and handle the event in `PlayerIdleHandleEvent()`:

```c
	case EVENT_CROUCH:
		ChangeState(object, STATE_CROUCH, deltaTime);
		break;
```

**Exiting Crouch** happens on `EVENT_NONE`, in `PlayerCrouchHandleEvent()` from step 4.

### 7. Update the input manager and `ExecuteCommand()`

In **`input_manager.c`**, set the bit while **C** is held:

```c
    if (IsKeyDown(KEY_C))
        command |= CROUCH;
```

In **`command.c`**, in `ExecuteCommand()`, straight after `object->inputAxis = axis;`:

```c
    // Crouch: while CROUCH is held, send only EVENT_CROUCH
    if (IsCommandActive(command, CROUCH))
    {
        HandleEvent(object, EVENT_CROUCH, deltaTime);
        return;
    }
```

> **Why the `return`?** Without it, `ExecuteCommand()` also sends `EVENT_NONE` while you hold C without moving. Crouch would stand you up, and Idle would crouch you again, **every frame**. Watch out for this with any state you hold a key for.

### 8. Tidy up everything else

- **`npc.c`:** in `InitNPCFSM()`, add `object->stateConfigs[STATE_CROUCH] = EMPTY_STATE_CONFIG;`. **Don't skip this one.** Without it, that slot is full of random memory, and the game crashes when it closes.
- **Every `switch (event)`** in `player.c` and `npc.c` needs a `case EVENT_CROUCH:` with the ignored events. The compiler warns you about each one it finds; that's why the switches list every event.
- **`hud.c` (optional):** show it on screen:

  ```c
  DrawActiveCommand(hud->lastCommand, CROUCH, "CROUCH", &y, PURPLE);
  ```

Build it, hold C and watch the console:

```
Player Hero <- EXIT <- Idle
Player Hero -> ENTER -> Crouch
```

Try crouching while you move, and moving while you crouch. Neither works yet, because only Idle can go to Crouch, and Crouch can only go to Idle. Whether they **should** work is a design decision. If they should, add the transitions.

---

## The tasks

Already working in the starter code:

- [x] Player states: Idle, Moving, Firing, Jumping, Dead and Respawn
- [x] NPC states: Idle, Firing and Dead
- [x] Entry, Update and Exit for every state, with a colour and a "breathing" effect so you can see which state is active
- [x] Firing and Jumping end on a timer
- [x] Collision with the NPC: both are pushed apart and lose health
- [x] Health bars above both objects
- [x] A HUD showing the command bits, the player's current state and a jump counter
- [x] Every state change printed to the console, and the command bits printed when they change

### Task 1: Explore the starter code

Run it, then find:

1. in `player.c`, the transition table (`…ValidTransitions[]`) for each state,
2. in `command.c`, where `ExecuteCommand()` turns command bits into events,
3. in `fsm.c`, where `ChangeState()` calls `Exit()` and then `Entry()`, and where it refuses a transition,
4. in `player.c`, how `PlayerUpdateJumping()` decides the jump is over,
5. in `npc.c`, which states are set to `EMPTY_STATE_CONFIG`,
6. in `game.c`, where collisions start and end, and which events they send.

Then move, jump and fire, and watch the console. Each change prints two lines, for example:

```
Player Hero <- EXIT <- Idle
Player Hero -> ENTER -> Jumping
```

> Want to see every state and its table at once? Call `PrintStateConfigs(data->player->base.stateConfigs, STATE_COUNT);` at the end of `InitGame()`.

### Task 2: Jump while moving

Hold **D** to move right, then press **SPACE**. The player doesn't jump, and the console says:

```
Invalid state transition from Player_Moving to Player_Jumping
```

The player's state diagram has an arrow from Moving to Jumping, but the code doesn't.

1. Find out why. `PlayerMovingHandleEvent()` **asks** to change to Jumping, but the Moving transition table doesn't **allow** it.
2. Fix it by adding `STATE_JUMPING` to `movingValidTransitions[]` in `InitPlayerFSM()`.
3. Jump while moving. The player should jump in the direction it's moving, because `PlayerEnterJumping()` copies `inputAxis` into `direction`.

> **Two locks on every door.** The state's `HandleEvent` decides which state to **ask** for. The transition table decides whether that's **allowed**. Both have to agree. When a transition doesn't happen, check both.

Did you notice the HUD's **Jumps** counter went up even when the jump was refused? The HUD counts JUMP **commands**. Whether a jump really happens is up to the FSM. Change the HUD so it only counts real jumps: the mediator already passes it the state's name.

### Task 3: Die and respawn

Health goes down on every collision, but nothing ever sends `EVENT_DIE`. Health just stops at 0.

1. **Send the event.** In `game.c`, after `CollisionEntry()`, check each object's health. When it reaches 0, send the event:

   ```c
   if (data->player->base.health <= 0)
       HandleEvent(&data->player->base, EVENT_DIE, deltaTime);
   ```

   Do the same for the NPC.

2. **Make Dead last.** At the moment, `PlayerUpdateDie()` changes to Respawn on the very next frame, so you never see it. Use `timer` to stay dead for **2 seconds** first. `PlayerEnterDie()` already resets the timer.

3. **Make Respawn do something.** In `PlayerEnterRespawn()`, put the player back in the middle of the screen and set `health` back to 100. Stay in Respawn for **1 second**, then change to Idle.

4. **Bring the NPC back.** The NPC's Dead state only leaves on `EVENT_RESPAWN`. Send it from `NPCUpdateDead()` after a few seconds, and reset the NPC's health.

5. **Finish the handlers.** `PlayerDieHandleEvent()` and `PlayerRespawnHandleEvent()` ignore every event. Decide what, if anything, a dead player should react to. Can a respawning player be hurt?

> **Where does the code go?** Something that happens **once**, like resetting health, goes in `Entry()` or `Exit()`. Something that happens **over time**, like a countdown, goes in `Update()`. Putting a one-off in `Update()` runs it 60 times a second.

### Task 4: Add a new state (Dashing)

Using the Crouch walkthrough as your guide, add a **Dashing** state: a short, fast burst in the direction the player is moving.

1. Add `STATE_DASHING` to `fsm.h`, **before** `STATE_COUNT`.
2. Add a `DASH` command, an `EVENT_DASH` event, and the key binding (e.g. **E**, with `IsKeyPressed`).
3. Write the state functions in `player.h` and `player.c`:

   ```c
   void PlayerDashingHandleEvent(GameObject *object, Event event, float deltaTime);
   void PlayerEnterDashing(GameObject *object, float deltaTime);
   void PlayerUpdateDashing(GameObject *object, float deltaTime);
   void PlayerExitDashing(GameObject *object, float deltaTime);
   ```

4. Configure the state in `InitPlayerFSM()`:

   ```c
   State dashingValidTransitions[] = {STATE_IDLE, STATE_MOVING, STATE_DEAD};
   object->stateConfigs[STATE_DASHING].name = "Player_Dashing";
   // ... set the function pointers
   StateTransitions(&object->stateConfigs[STATE_DASHING], dashingValidTransitions, 3);
   ```

5. Decide which states can **start** a dash, and add `STATE_DASHING` to their tables.
6. In `PlayerUpdateDashing()`, move fast (say 600 units a second) for 0.15 seconds, then go back to Moving or Idle, like `PlayerUpdateFiring()` does.
7. Don't forget step 8 of the walkthrough: the NPC's `EMPTY_STATE_CONFIG` and every `switch`.

### Task 5: Add a new command (Dodge)

Not every command needs its own state. Add **DODGE**: a quick sidestep that the current state handles itself.

1. Add the command flag in `command.h`, and update `COMMAND_COUNT`:

   ```c
   DODGE = 1 << 8,  // or the next free bit
   ```

2. Add the input binding in `input_manager.c`:

   ```c
   if (IsKeyPressed(KEY_LEFT_CONTROL))
       command |= DODGE;
   ```

3. Send an event in `ExecuteCommand()` in `command.c`:

   ```c
   if (IsCommandActive(command, DODGE))
       HandleEvent(object, EVENT_DODGE, deltaTime);
   ```

4. Add `EVENT_DODGE` to `events.h`, and handle it in the state functions. In Idle and Moving, move the player a short fixed distance sideways. In Jumping, ignore it.

When should something be a **state**, and when is an **event** handled inside a state enough? Write one sentence on it in your comments.

### Task 6: Change the collision response

Experiment with how collisions behave:

- Change the damage values (`DAMAGE_DEFAULT` in `constants.h`).
- Change the pushback distance (`PUSHBACK_DISTANCE`).
- Make collisions change state: the FSMs already receive `EVENT_COLLISION_START` and `EVENT_COLLISION_END`, but every handler ignores them. Make the NPC start firing when the player touches it.
- `CollisionExit()` in `collision.c` sets both colours directly, behind the FSM's back. Move the colour change into a state instead.

### Task 7: Give the NPC a brain

Watch the NPC for a few seconds. It fires almost non-stop. `PollAI()` picks a random command **every frame**, so FIRE comes up within a few frames every time the NPC returns to Idle.

1. Make `PollAI()` decide only **once a second**. Keep the time of the last decision in a `static double` and compare it with `GetTime()`. Between decisions, return `NONE`.
2. Use the NPC's `aggression` (0 to 100) to decide how likely it is to fire. Pass it to `PollAI()` as a parameter, so the AI manager still doesn't need to know what an NPC is.
3. Then try some patterns in `ai_manager.c`. Each will need more states for the NPC:
   - **Patrol:** move back and forth between two points.
   - **Chase:** move towards the player when it's in range.
   - **Flee:** run away when health is low.
   - **Wander:** random movement.

### Task 8: Command combos

Make a sequence of inputs trigger something special. For example, **double-tap A** within 0.3 seconds to dash left:

```c
// Double-tap detection
static double lastTapTime = 0.0;

if (IsKeyPressed(KEY_A))
{
    double now = GetTime();
    if (now - lastTapTime < DOUBLE_TAP_WINDOW)   // e.g. #define DOUBLE_TAP_WINDOW 0.3
    {
        // Trigger dash left
    }
    lastTapTime = now;
}
```

Combos need memory between frames, so keep the last tap time in a `static` variable or in a struct, never in a normal local variable.

### Task 9: Visual effects

Give the player more feedback:

- a trail while moving,
- an impact effect on collision,
- different animations for each state (Idle already "breathes"),
- a better health bar: change colour as health drops.

Each effect belongs to a state. Start it in `Entry()`, run it in `Update()`, and stop it in `Exit()`.

### Task 10: Stretch goals

- **Spend stamina and mana:** jumping costs stamina and firing costs mana. Both refill in Idle using `deltaTime`. When there isn't enough, the state's `HandleEvent` doesn't ask for the change.
- **Show the NPC's state:** the HUD only shows the player's. Draw `stateConfigs[currentState].name` above each object, or give the NPC's mediator a HUD of its own.
- **Go back to the previous state:** `previousState` is stored but never used. Use it so that Firing returns to whichever state the player was in before.
- **Load the transition tables from a file:** read `nextStates` from a text file at start-up, so a designer can change what follows what without recompiling.

---

## Cheat sheet

```c
// The FSM functions (fsm.c)
HandleEvent(obj, EVENT_JUMP, deltaTime);   // send an event to the current state
ChangeState(obj, STATE_IDLE, deltaTime);   // ask to change state; false if not allowed
CanEnterState(obj, STATE_DEAD);            // would that change be allowed?
UpdateState(obj, deltaTime);               // run the current state's Update
PrintStateConfigs(obj->stateConfigs, STATE_COUNT);   // print every state and its table

// Where am I?
obj->currentState                          // e.g. STATE_JUMPING
obj->stateConfigs[obj->currentState].name  // e.g. "Player_Jumping"
obj->previousState                         // the state before this one
```

A state's event handler, with a new event:

```c
void PlayerCustomHandleEvent(GameObject *object, Event event, float deltaTime)
{
    switch (event)
    {
    case EVENT_CUSTOM:
        ChangeState(object, STATE_CUSTOM, deltaTime);
        break;
    // Ignore every other event: list each one
    case EVENT_NONE:
    case EVENT_MOVE:
    // ...and the rest
        break;
    }
}
```

A state that ends on a timer:

```c
void PlayerUpdateCustom(GameObject *object, float deltaTime)
{
    object->timer += deltaTime;   // timer was reset to 0 in Entry()

    if (object->timer >= CUSTOM_DURATION)
        ChangeState(object, STATE_IDLE, deltaTime);
}
```

Bit flags and input, from the last practical:

```c
c |= JUMP;                     // turn a bit ON
c &= ~JUMP;                    // turn a bit OFF
if (c & JUMP) { }              // is the bit on? (what IsCommandActive does)

IsKeyDown(KEY_C);              // true every frame the key is held (crouch, move)
IsKeyPressed(KEY_E);           // true once, on the frame it goes down (dash, dodge)
double now = GetTime();        // seconds since the game started (timers, combos)
```

> The starter code lists every event in each `switch` instead of using `default:`. That way the compiler warns you when you add an event and forget a handler. With `default:`, it can't.

---

## When things go wrong

Don't panic. Nine times out of ten it's one of these:

| You see… | What's going on | Fix |
|---|---|---|
| `raylib.h: No such file or directory` | make can't find raylib | Run the setup script again (Step 2) |
| `Invalid state transition from A to B` | The state asked for B, but B isn't in A's table | Add B to A's `…ValidTransitions[]`, or stop asking for it (Task 2) |
| Nothing happens, and no message either | The state's `HandleEvent` ignores that event | Add a `case` that calls `ChangeState()` |
| The state flips in and out every frame | A held key, plus `EVENT_NONE` sent in the same frame | Send only the held key's event, like Crouch does (walkthrough step 7) |
| The state changes, but the player looks the same | `Entry()` doesn't set a colour, or it's not in the config | Check the config in `Init…FSM()` points at the right functions |
| The state never ends | Its `Update()` never calls `ChangeState()`, or the timer isn't reset | Reset `timer` in `Entry()`, add `deltaTime` in `Update()` |
| Something happens 60 times a second | One-off code in `Update()` | Move it to `Entry()` or `Exit()` |
| The terminal scrolls non-stop | A `printf` in an `Update()` or `HandleEvent` function | Print in `Entry()` and `Exit()` only |
| `free(): invalid pointer`, or a crash when the game closes | A new state has no config in `npc.c`, so its `nextStates` is random memory | Set it to `EMPTY_STATE_CONFIG` (walkthrough step 8) |
| `enumeration value 'EVENT_CROUCH' not handled in switch` | A new event isn't in every switch | Add the `case` to each handler |
| A new command does nothing | `COMMAND_COUNT` not updated, or the bit isn't checked in `ExecuteCommand()` | Check `command.h`, `input_manager.c` **and** `command.c` |
| Dash fires every frame while the key is held | `IsKeyDown` used for a one-off action | Use `IsKeyPressed` |
| The NPC fires all the time | `PollAI()` chooses every frame | Task 7 |
| `Emscripten not found in …` (Linux) | Emscripten isn't in `~/emsdk` | Run the setup script (Step 2) |
| `emcc: No such file or directory` (Windows) | Emscripten isn't loaded in this terminal | Close w64devkit and open it again |
| `Address already in use` | A web server from earlier is still running | `make stop_web`, or use another port: `make run_web WEB_PORT=8080` |
| Browser says it can't connect | The page opened before the server was ready | Refresh the page |
| The program still won't build | Old leftover files | `make clean`, then `make` again |

Still stuck? Read the **first** error message, not the last. The first one is usually the real problem; the rest are just complaining about it.

---

## Further reading

- [State](https://gameprogrammingpatterns.com/state.html) in *Game Programming Patterns* by Robert Nystrom: the classic explanation, starting with exactly the `bool` mess this practical avoids
- [State](https://refactoring.guru/design-patterns/state), [Command](https://refactoring.guru/design-patterns/command) and [Mediator](https://refactoring.guru/design-patterns/mediator) on Refactoring.Guru: diagrams and plain-English explanations
- [raylib cheatsheet](https://www.raylib.com/cheatsheet/cheatsheet.html) and [examples](https://www.raylib.com/examples.html)
- [cute_c2](https://github.com/RandyGaul/cute_headers/blob/master/cute_c2.h) by Randy Gaul: the collision library, with its documentation at the top of the header

---

## Before you submit

- [ ] It compiles and runs with no errors or warnings
- [ ] Crouch works, following the walkthrough
- [ ] The player can jump while moving, and no `Invalid state transition` appears in normal play
- [ ] The HUD's jump counter only counts jumps that really happened
- [ ] The player and the NPC die at 0 health, stay dead for a while and come back with full health
- [ ] Dashing is a state, and Dodge is handled inside states
- [ ] The NPC decides once a second, using its aggression, and has at least one AI pattern
- [ ] The game still closes cleanly, with no crash
- [ ] The state diagrams match your transition tables (click a diagram to edit it in the Mermaid Live Editor)
- [ ] Your code is commented and tidy

Commit early and commit often. Unlike the player, your deadline has no Respawn state.

Best of luck. Now go make something with a bit of character.