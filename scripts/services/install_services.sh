#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SERVICES_DIR="${SCRIPT_DIR}"

if [ $EUID -ne 0 ]; then
    echo "Error: must be run as root" >&2
    exit 1
fi

echo "Installing scripts from ${SERVICES_DIR}"
for script in "${SERVICES_DIR}"/*.sh; do
    [ -f "$script" ] || continue
    name=$(basename "$script")
    cp "$script" "/usr/local/bin/${name}" || { echo "Error: failed to copy ${name}" >&2; exit 1; }
    chmod +x "/usr/local/bin/${name}"
    echo "  installed ${name}"
done

echo "Installing systemd services"
for service in "${SERVICES_DIR}"/*.service; do
    [ -f "$service" ] || continue
    name=$(basename "$service")
    cp "$service" "/etc/systemd/system/${name}" || { echo "Error: failed to copy ${name}" >&2; exit 1; }
    echo "  installed ${name}"
done

echo "Reloading systemd"
systemctl daemon-reload

echo "Enabling and starting services"
for service in "${SERVICES_DIR}"/*.service; do
    [ -f "$service" ] || continue
    name=$(basename "$service")
    systemctl enable "$name"
    systemctl start "$name"
    echo "  enabled and started ${name}"
done

echo "Done"