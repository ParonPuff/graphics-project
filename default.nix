let
  pkgs = import <nixpkgs> { };
in
{
  project = pkgs.callPackage ./project.nix { };
}
