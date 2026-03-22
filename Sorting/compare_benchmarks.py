from __future__ import annotations

import csv
import subprocess
import sys
from collections import defaultdict
from io import StringIO
from pathlib import Path

ROOT = Path(__file__).resolve().parent
REPEATS = 1
FAST_KEYS = {"shell", "merge", "heap", "quick", "radix", "bucket", "baseline"}
SLOW_KEYS = {"bubble", "selection", "insertion"}
BASELINE_KEYS = {"qsort", "arrays", "python"}
MODE = "fast"

COMMANDS = {
    "C": ["../bin/sorting_demo", "benchmark-tsv", str(REPEATS)],
    "Java": ["java", "-cp", "bin", "SortingDemo", "benchmark-tsv", str(REPEATS)],
    "Python": ["python3", "sorting_demo.py", "benchmark-tsv", str(REPEATS)],
}

WORKDIRS = {
    "C": ROOT / "C",
    "Java": ROOT / "Java",
    "Python": ROOT / "Python",
}

BUILD_STEPS = [
    (["make", "demo"], ROOT),
    (["javac", "-d", "bin", "src/SortingDemo.java"], ROOT / "Java"),
]


def run_command(command: list[str], cwd: Path) -> str:
    completed = subprocess.run(command, cwd=cwd, check=True, capture_output=True, text=True)
    return completed.stdout


def parse_tsv_output(output: str) -> list[dict[str, str]]:
    lines = [line for line in output.splitlines() if line.strip()]
    return list(csv.DictReader(StringIO("\n".join(lines)), delimiter="\t"))


def normalize_algorithm_key(row: dict[str, str]) -> str:
    key = row["key"]
    if key in BASELINE_KEYS:
        return "baseline"
    return key


def normalize_algorithm_name(row: dict[str, str]) -> str:
    key = normalize_algorithm_key(row)
    if key == "baseline":
        return "Standard library sort"
    return row["name"]


def include_key(key: str) -> bool:
    if MODE == "full":
        return True
    if MODE == "slow-only":
        return key in SLOW_KEYS
    return key in FAST_KEYS


def load_results() -> list[dict[str, str]]:
    for command, cwd in BUILD_STEPS:
        subprocess.run(command, cwd=cwd, check=True)

    rows: list[dict[str, str]] = []
    for language, command in COMMANDS.items():
        output = run_command(command, WORKDIRS[language])
        parsed = parse_tsv_output(output)
        for row in parsed:
            row["normalized_key"] = normalize_algorithm_key(row)
            row["normalized_name"] = normalize_algorithm_name(row)
            if include_key(row["normalized_key"]):
                rows.append(row)
    return rows


def print_grouped_report(rows: list[dict[str, str]]) -> None:
    grouped: dict[str, list[dict[str, str]]] = defaultdict(list)
    algorithm_names: dict[str, str] = {}

    for row in rows:
        grouped[row["normalized_key"]].append(row)
        algorithm_names[row["normalized_key"]] = row["normalized_name"]

    print("Cross-language benchmark comparison")
    print("===================================")
    print(f"Mode: {MODE}")
    print(f"Runs per testcase: {REPEATS}\n")

    for algorithm_key in sorted(grouped.keys(), key=lambda key: algorithm_names[key].lower()):
        entries = sorted(grouped[algorithm_key], key=lambda row: int(row["average_us"]))
        print(algorithm_names[algorithm_key])
        for index, row in enumerate(entries, start=1):
            print(f"{index}. {row['language']:<7} {int(row['average_us']):>10} usec  ({row['complexity']})")
        print()


def print_matrix(rows: list[dict[str, str]]) -> None:
    grouped: dict[str, dict[str, str]] = defaultdict(dict)
    names: dict[str, str] = {}

    for row in rows:
        key = row["normalized_key"]
        grouped[key][row["language"]] = row["average_us"]
        names[key] = row["normalized_name"]

    print("Summary matrix (average usec)")
    print("=============================")
    print(f"{'Algorithm':<24} {'C':>12} {'Java':>12} {'Python':>12}")
    for key in sorted(grouped.keys(), key=lambda item: names[item].lower()):
        print(
            f"{names[key]:<24}"
            f" {grouped[key].get('C', '-'):>12}"
            f" {grouped[key].get('Java', '-'):>12}"
            f" {grouped[key].get('Python', '-'):>12}"
        )
    print()


def parse_args(argv: list[str]) -> int:
    global REPEATS, MODE
    for arg in argv[1:]:
        if arg == "--full":
            MODE = "full"
        elif arg == "--slow-only":
            MODE = "slow-only"
        elif arg == "--fast":
            MODE = "fast"
        else:
            try:
                REPEATS = int(arg)
                if REPEATS <= 0:
                    raise ValueError
            except ValueError:
                print(f"Invalid argument: {arg}", file=sys.stderr)
                return 1

    COMMANDS["C"][-1] = str(REPEATS)
    COMMANDS["Java"][-1] = str(REPEATS)
    COMMANDS["Python"][-1] = str(REPEATS)
    return 0


def main() -> int:
    if parse_args(sys.argv) != 0:
        return 1

    rows = load_results()
    print_grouped_report(rows)
    print_matrix(rows)

    if MODE == "fast":
        print("Fast mode skips bubble, selection, and insertion sort.")
        print("Use --full to include everything or --slow-only to compare just the quadratic sorts.\n")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
