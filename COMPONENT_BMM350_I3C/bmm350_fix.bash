#!/bin/bash

# This script is intended to workaround the issue where the BMM350 sensor initialization fails,
# when using the I3C interface due to a soft reset, causing the sensor interface to default to I2C.
# This issue is related to the line of code at https://github.com/boschsensortec/BMM350_SensorAPI/blob/main/bmm350.c#L219.
# This script comments out the a line which is causing the issue.
# Expected use:
#     ./bmm350_fix.bash "$(SEARCH_BMM350_SensorAPI)/bmm350.c"

file="$1"
search="soft_reset = BMM350_CMD_SOFTRESET"
replace="            // soft_reset = BMM350_CMD_SOFTRESET;"
new_contents=""

if [ ! -f "$file" ]; then
    echo "Error: File '$file' not found."
    exit 1
fi

while IFS= read -r line
do
    if [[ "$line" == *"$search"* ]]; then
        printf -v new_contents '%s%s\n' "$new_contents" "$replace"
    else
        printf -v new_contents '%s%s\n' "$new_contents" "$line"
    fi
done < "$file"

printf "%s" "$new_contents" > "$file"