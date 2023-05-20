use std::cmp::Ordering;
use std::collections::{BinaryHeap, HashMap};
use std::error::Error;

fn get_chars_frequency(text: impl AsRef<str>) -> HashMap<char, u32> {
	let mut frequency_table: HashMap<char, u32> = HashMap::new();

	for char in text.as_ref().chars() {
		frequency_table
			.entry(char)
			.and_modify(|v| *v += 1)
			.or_insert(1);
	}

	return frequency_table;
}

const BRANCH_SYMBOL: char = '$';

#[derive(Debug, Eq, PartialEq)]
struct TreeNode {
	symbol: char,
	frequency: u32,

	left: Option<Box<TreeNode>>,
	right: Option<Box<TreeNode>>,
}

impl TreeNode {
	fn new(symbol: char, frequency: u32) -> Self {
		TreeNode {
			symbol,
			frequency,
			left: None,
			right: None,
		}
	}
}

impl Default for TreeNode {
	fn default() -> Self {
		TreeNode {
			symbol: BRANCH_SYMBOL,
			frequency: 0,
			left: None,
			right: None,
		}
	}
}

impl Ord for TreeNode {
	fn cmp(&self, other: &Self) -> Ordering {
		other
			.frequency
			.cmp(&self.frequency)
			.then_with(|| Ordering::Less)
	}
}

impl PartialOrd for TreeNode {
	fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
		Some(self.cmp(other))
	}
}

impl From<&HashMap<char, u32>> for Box<TreeNode> {
	fn from(frequency_map: &HashMap<char, u32>) -> Self {
		let mut heap: BinaryHeap<Box<TreeNode>> = BinaryHeap::with_capacity(frequency_map.len());

		for (&symbol, &frequency) in frequency_map.iter() {
			heap.push(Box::new(TreeNode::new(symbol, frequency)));
		}

		while heap.len() > 1 {
			let left = heap.pop().unwrap();
			let right = heap.pop().unwrap();

			let mut node = Box::new(TreeNode::new(
				BRANCH_SYMBOL,
				left.frequency + right.frequency,
			));
			node.right = Some(right);
			node.left = Some(left);

			heap.push(node);
		}

		return heap.pop().unwrap();
	}
}

impl From<&HashMap<char, String>> for Box<TreeNode> {
	fn from(coding_map: &HashMap<char, String>) -> Self {
		let mut tree_root = Box::new(TreeNode::default());
		let mut curr_node = &mut tree_root;

		for (symbol, code) in coding_map.iter() {
			for ch in code.chars() {
				match ch {
					'0' => {
						curr_node = curr_node.left.get_or_insert(Box::new(TreeNode::default()));
					}
					'1' => {
						curr_node = curr_node.right.get_or_insert(Box::new(TreeNode::default()));
					}
					_ => panic!("Invalid binary char"),
				}
			}

			curr_node.symbol = *symbol;
			curr_node = &mut tree_root;
		}

		return tree_root;
	}
}

fn coding_map_from_tree(root: &Box<TreeNode>, chars_count: usize) -> HashMap<char, String> {
	let mut map = HashMap::with_capacity(chars_count);

	fn build(map: &mut HashMap<char, String>, node: &Box<TreeNode>, code: String) {
		if node.right.is_none() && node.left.is_none() {
			map.insert(node.symbol, code);
			return;
		}

		if let Some(left) = &node.left {
			build(map, left, code.clone() + "0");
		}
		if let Some(right) = &node.right {
			build(map, right, code.clone() + "1");
		}
	}

	build(&mut map, root, String::new());

	map
}

pub fn compress(text: impl AsRef<str>) -> (String, HashMap<char, String>) {
	let frequency_map = get_chars_frequency(text.as_ref());

	let tree_root = Box::<TreeNode>::from(&frequency_map);

	let coding_map = coding_map_from_tree(&tree_root, frequency_map.len());

	let compressed = text
		.as_ref()
		.chars()
		.map(|c| coding_map[&c].clone())
		.collect::<String>();

	return (compressed, coding_map);
}

pub fn uncompress(
	bits: impl AsRef<str>,
	coding_map: &HashMap<char, String>,
) -> Result<String, Box<dyn Error>> {
	let tree_root = Box::<TreeNode>::from(coding_map);

	let mut curr_node = &tree_root;
	let mut result = String::new();

	for bit in bits.as_ref().chars() {
		match bit {
			'0' => {
				curr_node = curr_node
					.left
					.as_ref()
					.ok_or::<Box<dyn Error>>("Invalid tree".into())?;

				if curr_node.symbol != BRANCH_SYMBOL {
					result += &curr_node.symbol.to_string();
					curr_node = &tree_root;
				}
			}
			'1' => {
				curr_node = curr_node
					.right
					.as_ref()
					.ok_or::<Box<dyn Error>>("Invalid tree".into())?;

				if curr_node.symbol != BRANCH_SYMBOL {
					result += &curr_node.symbol.to_string();
					curr_node = &tree_root;
				}
			}
			_ => return Err("Invalid bit".into()),
		};
	}

	return Ok(result);
}
