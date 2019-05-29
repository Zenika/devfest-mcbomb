This folder contains lot of sample written to test some things.

- `first_version`: is the first version of the bomb to defuse.
- `wires`: loop and display the state of wires. `1` means plugged, `0` means unplugged.
- `display`: just make a countdown from 60s to 0s. accelerate the countdown when pushing the button.
- `micros`: experiment the "multithreading" based on `micros()` function.
- `music`: experiment playing music by using `tone()` and `noTone()` functions.
- `non_blocking_music`: mix `micros` and `music` to play a music and countup seconds in parallel.
- `music_player`: separate the code of the song player and the arduino program to have a clearer code.
- `classes`: try to make a `SongPlayer` class but:
  - a class declared in an `.ino` file is not reachable from the main `ino` file.
  - a class declared in an `.cpp` file can't use the arduino functions (e.g. `tone()`, `noTone()` or `Serial.println()`).
- `beeps`: keep using the "multithreading" to have a beep each second and read a button to make the beeps twice faster.

