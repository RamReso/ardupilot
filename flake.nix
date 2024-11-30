{
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixpkgs-unstable";
  };

  outputs = { self, nixpkgs }:
    let
      system = "x86_64-linux";
      pkgs = nixpkgs.legacyPackages.${system};
      python = pkgs.python3.override {
        self = python;
        packageOverrides = pyfinal: pyprev: {
          empy = pyfinal.callPackage ./empy.nix { };
        };
      };
    in
    {
      devShell.${system} =
        pkgs.mkShell {
          packages = [
            (python.withPackages (python-pkgs: [
              python-pkgs.dronecan
              python-pkgs.empy
              python-pkgs.pexpect
              python-pkgs.ptyprocess
              python-pkgs.pymavlink
              python-pkgs.pyserial
              python-pkgs.setuptools
              python-pkgs.requests
              python-pkgs.sphinx
              python-pkgs.sphinx-rtd-theme
              python-pkgs.sphinxcontrib-youtube
              python-pkgs.sphinx-tabs
              python-pkgs.docutils
            ]))
          ] ++ [
            pkgs.gcc14
            pkgs.mavproxy
            pkgs.gdb
            pkgs.cmake
          ];
        };
    };
}
