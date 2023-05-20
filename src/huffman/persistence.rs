use std::error::Error;

pub fn parse_string_to_bytes(input: impl AsRef<str>) -> Result<Vec<u8>, Box<dyn Error>> {
	if input.as_ref().len() == 0 {
		return Ok(Vec::new());
	}

	let num_bytes = (input.as_ref().len() as f32 / 8 as f32).ceil() as usize;
	let mut bytes = vec![0; num_bytes];

	for (i, bit) in input.as_ref().chars().enumerate() {
		if bit == '1' {
			bytes[i / 8] |= 1 << (7 - (i % 8));
		} else if bit != '0' {
			return Err("Invalid bit".into());
		}
	}

	Ok(bytes)
}

pub fn parse_bytes_to_string(bytes: &[u8]) -> String {
	let mut result = String::with_capacity(bytes.len() * 8);

	for byte in bytes {
		for i in (0..8).rev() {
			let bit = (byte >> i) & 1;
			result += &bit.to_string();
		}
	}

	result
}
