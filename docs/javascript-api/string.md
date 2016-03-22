---
title: String
---

Smart.js has several non-standard extensions for `String.prototype` in
order to give a compact and fast API to access raw data obtained from
File, Socket, and hardware input/output such as I2C.
Smart.js IO API functions return
string data as a result of read operations, and that string data is a
raw byte array. ECMA6 provides `ArrayBuffer` and `DataView` API for dealing
with raw bytes, because strings in JavaScript are Unicode. That standard
API is too bloated for the embedded use, and does not allow to use handy
String API (e.g. `.match()`) against data.

Smart.js internally stores strings as byte arrays. All strings created by the
String API are UTF8 encoded. Strings that are the result of
input/output API calls might not be a valid UTF8 strings, but nevertheless
they are represented as strings, and the following API allows to access
underlying byte sequence:

- `String.prototype.at(position) -> number or NaN`: Return byte at index
  `position`. Byte value is in 0,255 range. If `position` is out of bounds
  (either negative or larger then the byte array length), NaN is returned.
  Example: `"ы".at(0)` returns 0xd1.

- `String.prototype.blen -> number`: Return string length in bytes.  Example:
  `"ы".blen` returns 2. Note that `"ы".length` is 1, since that string consists
  of a single Unicode character (2-byte).

