import argparse
# from distutils import dir_util
import sys
import os
import logging

_logger = logging.getLogger(__name__)
logging.basicConfig(level=logging.INFO)

parser = argparse.ArgumentParser(
    description='Clone repositories and add them to addons_path.')

parser.add_argument(
    'clone_path', metavar='--clone_path', type=str, help='Clone Path')
# TODO implementar optioncal para copiar todos addons a un dir addons_path
# parser.add_argument(
#     'addons_path', metavar='--addons_path', type=str, help='Addons Path')
parser.add_argument(
    "-S", "--sources_file", dest="sources_file", help="Sources File",
    default='sources.txt', type=file)
parser.add_argument(
    "--depth", dest="depth", help="Git Depth", type=int)

args = parser.parse_args()

depth = args.depth

# check addons path exist
clone_path = args.clone_path
# addons_path = args.addons_path

if not os.path.isdir(clone_path):
    _logger.error('Can not locate clone_path %s' % clone_path)
    sys.exit()

_logger.info('Using clone path %s' % clone_path)

for line in args.sources_file.readlines():
    _logger.info('Getting repository for line %s' % line)

    # check comma separated values
    line_vals = line.split(',')
    if len(line_vals) != 2:
        _logger.error('Line must have!: %s' % line)
        sys.exit()
    repository, path = tuple(line_vals)
    path = path.strip()
    path = os.path.join(clone_path, path)

    try:
        # check repository exist
        # TODO improove and use fabtools
        if os.path.isdir(path):
            _logger.warning('It seams repository %s already cloned')
            continue
        _logger.info('Cloning repository "%s" on path "%s"' % (
            repository, path))

        # clone
        if depth:
            os.system("git clone --single-branch --depth=%i %s %s" % (
                depth, repository, path))
        else:
            os.system("git clone --single-branch %s %s" % (repository, path))
        _logger.info('Repository %s cloned succesfully!' % repository)

        # INSTALL REPO PYTHON REQUIREMENTS
        requirements_txt = os.path.join(path, 'requirements.txt')
        _logger.info('Checking requirements.txt file exists on %s!' % (
            requirements_txt))
        if os.path.isfile(requirements_txt):
            _logger.info('Installing pip requirements of %s!' % (
                requirements_txt))
            os.system("pip install -r %s" % requirements_txt)
    except:
        _logger.warning('Could not clone repository: %s' % repository)

# if not os.path.isdir(addons_path):
#     _logger.error('Can not locate addons_path %s' % addons_path)
#     sys.exit()

# _logger.info('Using addons path %s' % addons_path)

    # if not os.path.isdir(path):
    #     _logger.error('Dir %s is not a valid directory' % path)

    # try:
    #     # copy addons tu addons_path
    #     dir_util.copy_tree(path, addons_path)
    #     _logger.info('Addons copied succesfully!')
    # except:
    #     _logger.warning('Coudl not copy Files from %s to %s' % (
    #         path, addons_path))
