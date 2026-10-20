import json
from pathlib import Path
import sys

def load_data(file_path):
    try:
        with open(file_path, 'r') as file:
            data = json.load(file)
            return data
    except FileNotFoundError:
        sys.exit(f"File {file_path} not found.")
    except json.JSONDecodeError:
        sys.exit(f"Error decoding JSON from {file_path}.")


def quote_field(value):
    # Match C++ std::quoted: escape backslashes and double quotes.
    if '\n' in value or '\r' in value:
        raise ValueError("Watch text fields must fit on one line.")
    return '"' + value.replace('\\', '\\\\').replace('"', '\\"') + '"'

if __name__ == "__main__":
    watches = load_data(Path(__file__).with_name("watches.json"))
    for watch in watches:
        # One watch per line: name brand price year movement.
        print(quote_field(watch["name"]), quote_field(watch["brand"]),
              watch["price"], watch["year"], quote_field(watch["movement"]))
