# NUCLEO-STM32L432KC Demo Project with Trice instrumentation

## Setup

See [../vsCode_Nucleo-L432KC_generated/ReadMe.md](../vsCode_Nucleo-L432KC_generated/ReadMe.md) as reference.

## Trice Command

```b
trice l -p JLINK -args "-Device STM32L432KC -If SWD -Speed 4000 -RTTChannel 0" -pf none -d16
```

You can simply say `make log`.
