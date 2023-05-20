mod huffman;

use std::time::Instant;

fn main() {
	let data = "Привет мир".repeat(1000000);

	let now = Instant::now();

	println!("{}", data.len());

	for _ in 0..1 {
		let (compressed, coding_map) = huffman::compress(&data);
		println!("{}", compressed.len());
		let _ = huffman::uncompress(&compressed, &coding_map);
	}

	let elapsed = now.elapsed();
	println!("Huffman new: {:.2?}", elapsed);
}
