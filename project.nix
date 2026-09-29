{
  stdenv,
  cmake,
  libGL,
  libGLU,
  glfw,
  glew,
  pkg-config,
}:
stdenv.mkDerivation {
  pname = "graphics";
  version = "0.1.0";
  src = ./.;

  nativeBuildInputs = [
    cmake
    pkg-config
  ];

  buildInputs = [
    libGL
    libGLU
    glfw
    glew
  ];

  cmakeFlags = [
    "-DCMAKE_BUILD_TYPE=Release"
  ];
  installPhase = ''
    runHook preInstall
    mkdir -p $out/bin
    cp studio3d $out/bin/
    runHook postInstall
  '';
}
