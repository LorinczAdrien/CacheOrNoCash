# CacheOrNoCash

![](https://github.com/LorinczAdrien/CacheOrNoCash/blob/master/Images/game_logo.png)

## Description

- CacheOrNoCash is a simple delivery game that was made to showcase my [splay tree](https://en.wikipedia.org/wiki/Splay_tree) implementation using the [SFML library](https://www.sfml-dev.org/).
- The basic idea is that you are a little robot, that's supposed to pick up and deliver what the CPU has processed. Esentially cache the information in either [L1, L2, L3 caches](https://en.wikipedia.org/wiki/CPU_cache).
- The twist is that cache storage is limited, L1 cache is the smallest and fastest, thus it gives the highest percentage reward, L2 rewards are lower and L3 rewards the lowest percentage.
- The main gimmick is that L1 and L2 are implemented using unordered maps (thus they can be seen as faster), while L3 is implemented using a splay tree (in the game it has 'infinite' storing capacity).
- The goal is to deliver the package as fast as you can and get the highest score by choosing the correct cache depending on the package. (A package has two main attributes: weight and score).

## Important

- Comments are in Hungarian.
- You need to set up your environment such that it can run [SFML](https://www.sfml-dev.org/) code.

## How to play

- On the start screen enter you name and press start.
- Controls:
  - W - Move up
  - S - Move down
  - A - Move left
  - D - Move right
  - E - Pick up/put down
