{
  inputs = {
    utils.url = "github:numtide/flake-utils";
  };
  outputs = { self, nixpkgs, utils }: utils.lib.eachDefaultSystem (system:
    let
      pkgs = nixpkgs.legacyPackages.${system};
      avrdude_usb = pkgs.avrdude.overrideAttrs (prev: {
        #cmakeFlags = [ "-DHAVE_LIBUSB=ON" ] ++ prev.cmakeFlags;
        buildInputs = prev.buildInputs ++ [ pkgs.libusb-compat-0_1 ];
      });
    in
    {
      devShell = pkgs.mkShell {
        buildInputs = with pkgs; [
          pkgs.pkgsCross.avr.buildPackages.gcc just cmake gnumake cmake-language-server ccls clang-tools clang
          avrdude_usb minicom
        ];
      };
    }
  );
}
