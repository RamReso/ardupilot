{ lib
, fetchPypi
, buildPythonPackage
, setuptools
, wheel
}:

buildPythonPackage rec {
  pname = "empy";
  version = "3.3.4";
  format = "setuptools";
  src = fetchPypi {
    inherit pname version;
    hash = "sha256-c6xJeFtgFHnfTqGKfHm8EwSop8NMArlHLPEgauiPAbM=";
  };

  # pyproject = true;
  build-system = [
    setuptools
    wheel
  ];
  meta = with lib; {
    homepage = "http://www.alcyone.com/software/empy/";
    description = "Templating system for Python";
    mainProgram = "em.py";
    license = licenses.lgpl21Only;
  };
}
