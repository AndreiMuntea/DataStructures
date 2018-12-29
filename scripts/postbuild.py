import sys
import os
import shutil
import time

def copy_file(source_file_path, source_file_name, destination_dir):
    destination_file = os.path.join(destination_dir, source_file_name)
    
    if not os.path.exists(destination_dir):
        os.makedirs(destination_dir)
    
    if os.path.isfile(destination_file):
        os.unlink(destination_file)
    
    shutil.copyfile(source_file_path, destination_file)

def copy(output, project_dir):
    destination = os.path.join(output, 'Include')
    
    for root, dirs, files in os.walk(project_dir):
        for file in files:
            if file.endswith('.h'):
                copy_file(os.path.join(root, file), file, destination)
    
    exit(0)
    
if __name__ == "__main__":
    if len(sys.argv) < 3 :
        print("Usage: {0} <{1}> <{2}>".format(sys.argv[0], 'output', 'project_dir'))
        sys.exit(1)
    
    copy(*sys.argv[1:])