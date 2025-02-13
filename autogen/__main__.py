import argparse
import jinja2
from pathlib import Path

def get_data(args):
    return {"hw_name": args.hw_name}

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("-t", dest="template_name", required=True)
    parser.add_argument("-n", dest="hw_name", required=True)
    parser.add_argument("-o", dest="output", required=True, metavar="DIR")

    args = parser.parse_args()

    data = get_data(args)

    template_dir = Path("autogen/templates", args.template_name)
    template_loader = jinja2.FileSystemLoader(searchpath=template_dir)
    env = jinja2.Environment(loader=template_loader)
    env.tests["contains"] = (lambda list, var: (var in list))

    output_dir = Path(args.output, data["hw_name"])
    output_dir.mkdir(parents=True, exist_ok=True)

    for template in template_dir.glob("**/*.jinja"):
        template_path = str(template.relative_to(template_dir))

        # Create the file text by rendering it with provided data (Must be dictionary)
        output = env.get_template(template_path).render(**data)

        # Create the output file name with provided data
        output_file_name = env.from_string(template_path).render(**data)

        output_file = output_dir / output_file_name.replace(".jinja", "")
            
        if not output_file.exists():
            output_text = env.get_template(template_path).render(**data)
            output_file.write_text(output_text)
            print(f"Generated: {output_file}")
        else:
            print(f"Skipped (already exists): {output_file}")

    print("Test bench generation complete.")

if __name__ == "__main__":
    main()
