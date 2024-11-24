# Cmder

[![builds.sr.ht status](https://builds.sr.ht/~ekoeppen/cmder.svg)](https://builds.sr.ht/~ekoeppen/cmder?)

`cmder` is a PlatformIO library to implement a simple, text based protocol to
control embedded devices.

The protocol is line based, and each line follows the following patter:

`<command>;<sequence number>;<number of messages>;<arg1>;<arg2>;...<LF>`

Fields are separated by semicolon, and all fields are optional except for the
first one, optional fields can be left empty. A line is terminated with the line
feed character (ASCII code 10).

In order to send multiline commands or receive multiline responses, each line
can carry a sequence number and the maximum number of lines for the command.
Lines do not have to be sent in sequence and can be interleaved in case multiple
commands or responses are sent at the same time.
