use std::cmp::Ordering;
use std::collections::{BinaryHeap, HashMap};

pub fn get_chars_frequency(string: &str) -> HashMap<char, u32> {
	let mut frequency_table: HashMap<char, u32> = HashMap::new();

	for char in string.chars() {
		frequency_table
			.entry(char)
			.and_modify(|v| *v += 1)
			.or_insert(1);
	}

	return frequency_table;
}

const BRANCH_SYMBOL: char = '$';

#[derive(Debug, Eq, PartialEq)]
pub struct TreeNode {
	pub symbol: char,
	pub frequency: u32,

	pub left: Option<Box<TreeNode>>,
	pub right: Option<Box<TreeNode>>,
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

#[derive(Debug)]
pub struct CodingMap {
	map: HashMap<char, String>,
}

impl CodingMap {
	fn build(self: &mut Self, node: &Box<TreeNode>, code: String) {
		if node.right.is_none() && node.left.is_none() {
			self.map.insert(node.symbol, code);
			return;
		}

		if let Some(l) = &node.left {
			self.build(l, code.clone() + "0");
		}
		if let Some(r) = &node.right {
			self.build(r, code.clone() + "1");
		}
	}

	pub fn from_tree(root: &Box<TreeNode>, capacity: usize) -> Self {
		let mut map = Self {
			map: HashMap::with_capacity(capacity),
		};

		map.build(root, "".to_string());

		return map;
	}
}
