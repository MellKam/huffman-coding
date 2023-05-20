mod huffman;

use huffman::*;

fn main() {
	let input = "Привет мир";

	let (compressed, coding_map) = compress(input);

	println!("{compressed} {coding_map:#?}");

	let output = uncompress(compressed, &coding_map).unwrap();

	println!("{output}");
}
