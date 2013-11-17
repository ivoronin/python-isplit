from distutils.core import setup, Extension

setup(name = 'isplit',
	  version = '0.1',
	  author = 'Ilya Voronin',
	  author_email = 'ivoronin@gmail.com',
	  url = 'https://github.com/ivoronin/python-isplit',
	  description = 'Generator version of string.split()',
	  license = 'GPLv2',
	  ext_modules = [Extension('isplit', sources=['isplit.c'])])