#!/bin/bash
set -euo pipefail

mkdir -p tests/data

echo "[*] Enabling auditd..."
sudo systemctl enable --now auditd

echo "[*] Installing audit rules for execve/open/openat..."
sudo auditctl -a always,exit -F arch=b64 -S execve -k exec-log
sudo auditctl -a always,exit -F arch=b64 -S open,openat -k open-log


echo "[*] Verifying sshd is running..."
sudo systemctl start sshd

echo "[*] Triggering SYSCALL, EXECVE, CWD..."
/bin/ls /tmp >/dev/null
sleep 1

echo "[*] Triggering OPEN, PATH, CWD..."
/bin/cat /etc/passwd >/dev/null
sleep 1

echo "[*] Triggering USER_CMD..."
sudo whoami >/dev/null
sleep 1

echo "[*] Triggering ADD_USER..."
sudo useradd --no-create-home --shell /sbin/nologin testaudituser || true
sleep 1

echo "[*] Triggering DEL_USER..."
sudo userdel testaudituser || true
sleep 1

echo "[*] Triggering keygen..."
ssh-keygen -q -N "" -f /tmp/testauditkey <<<y >/dev/null
rm -f /tmp/testauditkey /tmp/testauditkey.pub
sleep 1

echo "[*] Triggering audit config change..."
sudo auditctl -l >/dev/null
sleep 1

echo "[*] Extracting logs to test fixtures..."
sudo ausearch -k exec-log       > tests/data/syscall_execve_cwd.log || true
sudo ausearch -k open-log       > tests/data/open_path.log || true
sudo ausearch -m USER_CMD       > tests/data/user_cmd.log || true
sudo ausearch -m ADD_USER,DEL_USER > tests/data/user_add_del.log || true
sudo ausearch -m CONFIG_CHANGE  > tests/data/audit_config_change.log || true
sudo ausearch -m CRYPTO_KEY_USER > tests/data/crypto_key.log || true

echo "[✓] Done. Fixture logs saved to tests/data/"

