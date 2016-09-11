<?php
/*
Version: 1.0
Licence: GNU Lesser General Public License
Licence version: 3
http://www.gnu.org/licenses/lgpl.html
*/
class BMP_Loader {
	const BITMAPINFOHEADER = "VWidth/VHeight/vPlanes/vBitCount/VCompression/VImageSize/VXpixelsPerM/VYpixelsPerM/VColorsUsed/VColorsImportant";
	const BITMAPCOREHEADER = "vWidth/vHeight/vPlanes/vBitCount";
	public $error = false;
	private $data = array();
	
	function BMP_Loader($path = "") {
		if (strlen($path) > 0) $this->Load_BMP($path);
	}
	
	public function Load_BMP($path) {
		$fp = @fopen($path, "r");
		if ($fp === false) return false;
		$data = "";
		while (! feof($fp)) {
			$data .= fread($fp, 4096);
		}
		fclose($fp);
		$this->Load_BMP_Data($data);
	}
	
	public function Load_BMP_Data($data = "") {
		if (strlen($data) == 0) return false;
		$this->data['FILE_HEADER'] = unpack("vMagic/VSize/LReserved/VOffset/VHeaderSize", $data);
		$data = substr($data, 18);
		if ($this->data['FILE_HEADER']['HeaderSize'] == 12) {
			$this->data['BITMAP_HEADER'] = unpack(self::BITMAPCOREHEADER, $data);
		} else {
			$this->data['BITMAP_HEADER'] = unpack(self::BITMAPINFOHEADER, $data);
		}
		$this->data['BITMAP_HEADER']['HeaderSize'] = $this->data['FILE_HEADER']['HeaderSize'];
		unset($this->data['FILE_HEADER']['HeaderSize']);
		$data = substr($data, $this->data['FILE_HEADER']['Offset'] - 18);
		switch ($this->data['BITMAP_HEADER']['BitCount']) {
			case 1:
			case 4:
			case 8:
				$this->data['palette'] = array();
				$color_count = ($this->data['BITMAP_HEADER']['ColorsUsed'] > 0 ? $this->data['BITMAP_HEADER']['ColorsUsed'] : pow(2, $this->data['BITMAP_HEADER']['BitCount']));
				$p = 0;
				for ($i = 0; $i < $color_count; $i++) {
					$b = ord($data[$p]);
					$g = ord($data[$p + 1]);
					$r = ord($data[$p + 2]);
					$this->data['palette'][] = array($r, $g, $b);
					$p += 4;
				}
				$data = substr($data, $p);
		}
		$this->data['IMAGE'] = imagecreatetruecolor($this->data['BITMAP_HEADER']['Width'], $this->data['BITMAP_HEADER']['Height']);
		$bg_color = imagecolorallocate($this->data['IMAGE'], 255, 255, 255);
		imagefilledrectangle($this->data['IMAGE'], 0, 0, $this->data['BITMAP_HEADER']['Width'] - 1, $this->data['BITMAP_HEADER']['Height'] - 1, $bg_color);
		$p = 0;
		switch ($this->data['BITMAP_HEADER']['BitCount']) {
			case 32:
				for ($i = $this->data['BITMAP_HEADER']['Height'] - 1; $i >= 0; $i--) {
					for ($j = 0; $j < $this->data['BITMAP_HEADER']['Width']; $j++) {
						$b = ord($data[$p]);
						$g = ord($data[$p + 1]);
						$r = ord($data[$p + 2]);
						$a = ord($data[$p + 3]);
						$color = imagecolorallocatealpha($r, $g, $b, $a / 255 * 127);
						imagesetpixel($this->data['IMAGE'], $j, $i, $color);
						$p += 4;
					}
				}
				break;
			case 24:
				for ($i = $this->data['BITMAP_HEADER']['Height'] - 1; $i >= 0; $i--) {
					for ($j = 0; $j < $this->data['BITMAP_HEADER']['Width']; $j++) {
						$b = ord($data[$p]);
						$g = ord($data[$p + 1]);
						$r = ord($data[$p + 2]);
						$color = imagecolorallocate($this->data['IMAGE'], $r, $g, $b);
						imagesetpixel($this->data['IMAGE'], $j, $i, $color);
						$p += 3;
					}
					if ($p % 4 > 0) $p += 4 - ($p % 4);
				}
				echo "HAL2:" . (strlen($data) - $p);
				break;
			case 8:
				for ($i = $this->data['BITMAP_HEADER']['Height'] - 1; $i >= 0; $i--) {
					for ($j = 0; $j < $this->data['BITMAP_HEADER']['Width']; $j++) {
						$index = ord($data[$p]);
						$color = imagecolorallocate($this->data['IMAGE'], $this->data['palette'][$index][0], $this->data['palette'][$index][1], $this->data['palette'][$index][2]);
						imagesetpixel($this->data['IMAGE'], $j, $i, $color);
						$p++;
					}
					if ($p % 4 > 0) $p += 4 - ($p % 4);
				}
				break;
			case 4:
				$bit_p = 0;
				for ($i = $this->data['BITMAP_HEADER']['Height'] - 1; $i >= 0; $i--) {
					for ($j = 0; $j < $this->data['BITMAP_HEADER']['Width']; $j++) {
						$byte = ord($data[$p]);
						if ($bit_p % 2 == 0) {
							$index = $byte >> 4;
						} else {
							$index = $byte & 0x0F;
							$p++;
						}
						$bit_p++;
						$color = imagecolorallocate($this->data['IMAGE'], $this->data['palette'][$index][0], $this->data['palette'][$index][1], $this->data['palette'][$index][2]);
						imagesetpixel($this->data['IMAGE'], $j, $i, $color);
					}
					if ($p % 4 > 0) $p += 4 - ($p % 4);
				}
				break;
			case 1:
				$bit_p = 0;
				for ($i = $this->data['BITMAP_HEADER']['Height'] - 1; $i >= 0; $i--) {
					for ($j = 0; $j < $this->data['BITMAP_HEADER']['Width']; $j++) {
						$byte = ord($data[$p]);
						$index = ($byte >> (8 - ($bit_p + 1))) & 0x01;
						$bit_p++;
						$bit_p %= 8;
						if ($bit_p % 8 == 0) {
							$p++;
						}
						$color = imagecolorallocate($this->data['IMAGE'], $this->data['palette'][$index][0], $this->data['palette'][$index][1], $this->data['palette'][$index][2]);
						imagesetpixel($this->data['IMAGE'], $j, $i, $color);
					}
					if ($p % 4 > 0) {
						$p += 4 - ($p % 4);
						$bit_p = 0;
					}
				}
		}
		print_r($this->data);
		imagepng($this->data['IMAGE'], "./poba.png");
	}
	
	public function Get_BMP_Data() {
		return $this->data;
	}
	
	public function Get_Image() {
		return $this->data['IMAGE'];
	}
	
	public static function imagebmp($img, $dest) {
		$fp = @fopen($dest, "w");
		if (! $fp) return false;
		$width = imagesx($img);
		$height = imagesy($img);
		$row_length = 3 * $width;
		if ($row_length % 4 > 0) $row_length += 4 - ($row_length % 4);
		$size = $row_length * $height;
		$out = pack("ccVccccVVVVvvVVVVVV", 0x42, 0x4D, $size + 54, 84, 111, 109, 105, 54, 40, $width, $height, 1, 24, 0, $size, 0, 0, 0, 0);
		$num = fwrite($fp, $out);
		echo "BYTES: " . $num . "\n";
		$out = "";
		for ($i = $height - 1; $i >= 0; $i--) {
			$p = 0;
			for ($j = 0; $j < $width; $j++) {
				$color = imagecolorsforindex($img, imagecolorat($img, $j, $i));
				$out .= chr($color['blue']) . chr($color['green']) . chr($color['red']);
				$p += 3;
			}
			for ($pad = 0; $pad < $row_length - $p; $pad++) $out .= "T";
		}
		fwrite($fp, $out);
		fclose($fp);
		return true;
	}
}
?>