#!/usr/bin/env python3
"""Test runner for lang compiler tests"""
import xml.etree.ElementTree as ET
import subprocess
import os
import sys

def normalize_xml(xml_path):
    """Load and normalize XML for comparison"""
    try:
        tree = ET.parse(xml_path)
        root = tree.getroot()
        # Return as canonical string
        return ET.tostring(root, encoding='unicode')
    except Exception as e:
        print(f"Error parsing XML: {e}", file=sys.stderr)
        return None

def run_test(test_num):
    """Run a single test and return (passed, msg)"""
    test_name = f"test{test_num}"
    input_file = f"test/{test_name}.lang"
    output_file = f".test_output_{test_num}.xml"
    expected_file = f"test/{test_name}.correct"
    
    if not os.path.exists(input_file):
        return False, f"Input file not found: {input_file}"
    
    if not os.path.exists(expected_file):
        return False, f"Expected file not found: {expected_file}"
    
    # Run the compiler
    result = subprocess.run(
        ["./lang", input_file, output_file],
        stderr=subprocess.PIPE,
        stdout=subprocess.PIPE,
        cwd=os.getcwd()
    )
    
    if result.returncode != 0 and "syntax error" in result.stderr.decode():
        return False, f"Parser error: {result.stderr.decode().strip()[:80]}"
    
    # Compare outputs
    gen_xml = normalize_xml(output_file)
    exp_xml = normalize_xml(expected_file)
    
    if gen_xml is None or exp_xml is None:
        return False, "XML parsing failed"
    
    if gen_xml == exp_xml:
        return True, "OK"
    else:
        # Show first difference
        gen_lines = gen_xml.split('<')
        exp_lines = exp_xml.split('<')
        for i, (g, e) in enumerate(zip(gen_lines, exp_lines)):
            if g != e:
                return False, f"XML mismatch at element {i}: gen={g[:40]}, exp={e[:40]}"
        return False, f"XML length mismatch: {len(gen_xml)} vs {len(exp_xml)}"

# Run all tests
test_nums = list(range(1, 5)) + ['5a', '5b', '5c', '5d', '5e', '5f'] + list(range(6, 12))
passed = 0
failed = 0

for test_num in test_nums:
    ok, msg = run_test(test_num)
    status = "PASS" if ok else "FAIL"
    print(f"test{test_num:2s}: {status:4s} - {msg}")
    if ok:
        passed += 1
    else:
        failed += 1

print(f"\nSummary: {passed} passed, {failed} failed out of {passed+failed} tests")
