
Use Python 3.10 not 3.11, whisper will not work otherwise
Whisper must be installed from git, not the pip package, it is too old
- pip install git+https://github.com/openai/whisper.git
- pip install --upgrade --no-deps --force-reinstall git+https://github.com/openai/whisper.git

