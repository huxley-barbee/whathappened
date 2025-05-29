
# What Happened

[![CI](https://github.com/huxley-barbee/whathappened/actions/workflows/ci.yml/badge.svg)](https://github.com/huxley-barbee/whathappened/actions)
[![codecov](https://codecov.io/gh/huxley-barbee/whathappened/branch/main/graph/badge.svg)](https://codecov.io/gh/huxley-barbee/whathappened)
[![License: MIT](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![Packaging: DEB & RPM](https://img.shields.io/badge/packaging-deb%20%7C%20rpm-green.svg)](#packaging)

`whathappened` parses Linux Audit logs and reconstructs what happened — in plain language.

## Features

- Parses audit log lines or real-time netlink input (planned)
- Correlates multi-line records into sessions and events
- Narrates actions with templates and i18n
- Plugin architecture for handling AVC, SYSCALL, etc.
- Outputs pretty trees or machine-readable JSON
- Unit-tested and CI-verified with Codecov
- Supports DEB and RPM packaging

## Usage

```bash
whathappened /var/log/audit/audit.log
ausearch -ts today | whathappened
```

## Build

```bash
make
```

## Test

```bash
make test
```

## Packaging

```bash
dpkg-buildpackage -us -uc
rpmbuild -ba packaging/redhat/whathappened.spec
```


## Output Example

```
🟢 Session 1834 — user jdoe (UID 1000)
├── 📄 Executed: /usr/bin/vim /etc/passwd
│   └── CWD: /home/jdoe
├── ❌ Attempted unauthorized read: /root/secret
│   └── SELinux denial: user_t → admin_home_t (read)
└── 🌐 Downloaded remote script using curl
    └── URL: http://malicious.site/payload.sh

🔚 Summary: 3 commands run, 1 SELinux denial
```

## Architecture Overview

- `input/` — log file or (future) netlink readers
- `parser/` — audit line parser
- `explain/` — base explainer engine
- `plugins/` — modular type-specific explainers
- `narration/` — template-based string generation with i18n
- `output/` — human or JSON rendering
- `correlate/` — session tracker, process tree logic
- `tests/` — unit tests with cmocka
- `packaging/` — DEB and RPM metadata

## Testing

Unit tests are written using [cmocka](https://cmocka.org) and run via GitHub Actions.

```
make test
```

## Build and Install

### Dependencies

- GCC
- `cmocka` (`libcmocka-dev`)
- `make` or `cmake`

### Build

```
make
```

### Install

```
sudo make install
```

## Internationalization

Narration templates support multiple languages via `templates/en.json`, `templates/es.json`, etc.

Select language:

```
whathappened --lang en
```

## Future Roadmap

- [x] Session-level grouping
- [ ] Process tree correlation
- [ ] Multi-process activity detection (e.g., worker pools)
- [ ] Live mode via netlink audit socket
- [ ] Graphical output (DOT, HTML reports)

## Why Use What Happened?

- Tired of deciphering raw `audit.log` entries?
- Need to summarize user activity or investigate an incident?
- Want to plug audit intelligence into your pipeline or SIEM?

**This tool tells you what happened. In plain language.**

## License

MIT (or insert your license of choice)

## Contributing

PRs welcome! Unit tests are required for all new plugins or correlators.

---
