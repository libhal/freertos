# `libhal/freertos`

FreeRTOS conan package compatible with the libhal ecosystem.

## Using it with C++

Be warned that if you use `thread_local`, C++ exceptions, or are using a
library that uses thread local storage or exceptions, it is up to you to ensure
that thread local storage (TLS) is configured properly. If TLS is not
configured correctly, any access of the thread local variable or
`std::current_exception` will be undefined behavior.

## Contributing

See [`CONTRIBUTING.md`](CONTRIBUTING.md) for details.

## License

Apache 2.0; see [`LICENSE`](LICENSE) for details.
