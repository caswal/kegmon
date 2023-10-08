import os
import shutil
import re
#from SCons.Script import Import


Import("env")

try:
     import htmlmin
except ImportError:
    env.Execute(
        env.VerboseAction(
            '$PYTHONEXE -m pip install "htmlmin" ',
            "Installing htmlmin",
        )
    )
    import htmlmin




def minify_html(html_content):
    return htmlmin.minify(html_content, remove_comments=True, remove_empty_space=True, reduce_boolean_attributes=True)

def minify_and_write(src_path, dest_path):
    print(f"minify_and_write: {src_path}, {dest_path}")
    with open(src_path, 'r') as f:
        content = f.read()
    minimized_content = minify_html(content)
    with open(dest_path, 'w') as f:
        f.write(minimized_content)

project_config = env.GetProjectConfig()
scale_count = project_config.get("common_env_data", "scale_count")

print(f"Scale Count: {scale_count}")

if scale_count:
    src_folder = f"html/{scale_count}/"
    dest_folder = "html/"

    print(f"Removing .min.html files from /html/")

    # Remove existing .min.html files in dest_folder
    for filename in os.listdir(dest_folder):
        if filename.endswith('.min.htm'):
            os.remove(os.path.join(dest_folder, filename))


    # Minimize existing HTML files in dest_folder
    print(f"Minimizing {dest_folder} files")
    for filename in os.listdir(dest_folder):
        print(f"Minimizing {filename} file")
        if filename.endswith('.htm'):
            src_path = os.path.join(dest_folder, filename)
            min_filename = filename.replace('.htm', '.min.htm')
            dest_path = os.path.join(dest_folder, min_filename)
            minify_and_write(src_path, dest_path)

    print(f"Minimizing {src_folder} files and writing them to /html/")

    # Copy and minimize from src_folder to dest_folder
    if os.path.exists(src_folder):
        for filename in os.listdir(src_folder):
            if filename.endswith('.htm'):
                min_filename = filename.replace('.htm', '.min.htm')
                dest_path = os.path.join(dest_folder, min_filename)
                minify_and_write(os.path.join(src_folder, filename), dest_path)
            #else:
            #    shutil.copy(os.path.join(src_folder, filename), os.path.join(dest_folder, filename))
    