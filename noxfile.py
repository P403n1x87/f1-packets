import os
import sys
import tempfile

import nox

nox.options.sessions = ["tests"]


# ---- Configuration ----

SUPPORTED_PYTHON_VERSIONS = [
    "3.8",
    "3.9",
    "3.10",
]
REQUESTED_PYTHON_VERSION = os.getenv("PYTHON") or SUPPORTED_PYTHON_VERSIONS

PYTEST_OPTIONS = ["-vvvs"]


# ---- Helpers ----


def install_with_constraints(session, *args, **kwargs):
    with tempfile.NamedTemporaryFile() as requirements:
        session.run(
            "poetry",
            "export",
            "--dev",
            "--format=requirements.txt",
            "--without-hashes",
            f"--output={requirements.name}",
            external=True,
        )
        session.install(f"--constraint={requirements.name}", *args, **kwargs)


# ---- Sessions ----


@nox.session(python=REQUESTED_PYTHON_VERSION)
def tests(session):
    session.run("poetry", "install", "-vv", external=True)
    session.run("poetry", "run", "python", "-m", "pytest", *PYTEST_OPTIONS)
