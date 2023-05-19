mod huffman;

use huffman::*;

fn main() {
	let str = "hello world";

	let frequency_map = get_chars_frequency(str);

	let tree_root = Box::<TreeNode>::from(&frequency_map);

	let coding_map = CodingMap::from_tree(&tree_root, frequency_map.len());

	println!("{:#?}", coding_map);
}
