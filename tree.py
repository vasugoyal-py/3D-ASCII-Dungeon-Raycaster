from pathlib import Path


def main() -> None:
    root = Path(__file__).resolve().parent
    source_files = sorted(
        path
        for path in root.iterdir()
        if path.is_file() and path.suffix.lower() in {".c", ".h"}
    )

    output_path = root / "tree.txt"
    with output_path.open("w", encoding="utf-8") as output:
        for index, path in enumerate(source_files):
            content = path.read_text(encoding="utf-8")
            output.write(f"===== {path.name} =====\n")
            output.write(content)
            if not content.endswith("\n"):
                output.write("\n")
            if index != len(source_files) - 1:
                output.write("\n")


if __name__ == "__main__":
    main()
