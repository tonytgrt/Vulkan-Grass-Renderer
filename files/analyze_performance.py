#!/usr/bin/env python3
"""
Performance Analysis Script for Vulkan Grass Renderer
Analyzes CSV output from automated performance testing and generates visualization graphs.

Usage:
    python analyze_performance.py <csv_file_path>
"""

import sys
import os
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

def analyze_performance(csv_path):
    """
    Analyze performance test results and generate graphs.

    Args:
        csv_path: Path to the CSV file containing performance test results
    """
    # Check if file exists
    if not os.path.exists(csv_path):
        print(f"Error: File not found: {csv_path}")
        return

    # Read CSV
    print(f"Reading data from: {csv_path}")
    df = pd.read_csv(csv_path)

    # Get output path (same directory as CSV)
    output_dir = os.path.dirname(csv_path)
    csv_basename = os.path.splitext(os.path.basename(csv_path))[0]

    # Print summary statistics
    print("\n=== Performance Summary ===")
    print(f"Blade count range: {df['Blade Count'].min()} to {df['Blade Count'].max()}")
    print(f"\nAverage FPS by culling configuration:")
    for col in df.columns[1:]:
        avg_fps = df[col].mean()
        print(f"  {col}: {avg_fps:.2f} FPS")

    # Color scheme for different culling configurations
    colors = {
        'All Culling Off': '#e74c3c',      # Red
        'Only Orientation': '#3498db',      # Blue
        'Only View-Frustum': '#2ecc71',     # Green
        'Only Distance': '#f39c12',         # Orange
        'All Culling On': '#9b59b6'         # Purple
    }

    output_paths = []
    xticks = df['Blade Count'].values

    # ===== GRAPH 1: FPS vs Blade Count =====
    fig1, ax1 = plt.subplots(figsize=(10, 7))

    for col in df.columns[1:]:
        ax1.plot(df['Blade Count'], df[col], marker='o', linewidth=2.5,
                label=col, color=colors[col], markersize=8)

    ax1.set_xscale('log', base=2)
    ax1.set_xlabel('Blade Count (log scale)', fontsize=13, fontweight='bold')
    ax1.set_ylabel('FPS', fontsize=13, fontweight='bold')
    ax1.set_title('Vulkan Grass Renderer - FPS vs Blade Count', fontsize=14, fontweight='bold', pad=20)
    ax1.legend(loc='best', fontsize=11)
    ax1.grid(True, alpha=0.3, linestyle='--')
    ax1.set_xticks(xticks)
    ax1.set_xticklabels([f'2^{int(np.log2(x))}' for x in xticks], rotation=45)

    plt.tight_layout()
    output_path_1 = os.path.join(output_dir, f"{csv_basename}_fps_vs_blades.png")
    plt.savefig(output_path_1, dpi=300, bbox_inches='tight')
    output_paths.append(output_path_1)
    plt.close(fig1)

    # ===== GRAPH 2: Culling Performance Improvement =====
    fig2, ax2 = plt.subplots(figsize=(10, 7))

    # Calculate speedup relative to "All Culling Off"
    baseline = df['All Culling Off']
    speedup_data = {}

    for col in df.columns[1:]:
        if col != 'All Culling Off':
            speedup_data[col] = df[col] / baseline

    x_pos = np.arange(len(df['Blade Count']))
    width = 0.2

    for i, (col, speedup) in enumerate(speedup_data.items()):
        offset = (i - len(speedup_data)/2 + 0.5) * width
        ax2.bar(x_pos + offset, speedup, width, label=col, color=colors[col], alpha=0.8)

    ax2.axhline(y=1.0, color='red', linestyle='--', linewidth=2, label='Baseline (No Culling)')
    ax2.set_xlabel('Blade Count', fontsize=13, fontweight='bold')
    ax2.set_ylabel('Speedup vs No Culling', fontsize=13, fontweight='bold')
    ax2.set_title('Vulkan Grass Renderer - Culling Performance Improvement', fontsize=14, fontweight='bold', pad=20)
    ax2.set_xticks(x_pos)
    ax2.set_xticklabels([f'2^{int(np.log2(x))}' for x in df['Blade Count']], rotation=45)
    ax2.legend(loc='best', fontsize=11)
    ax2.grid(True, alpha=0.3, axis='y', linestyle='--')

    plt.tight_layout()
    output_path_2 = os.path.join(output_dir, f"{csv_basename}_culling_improvement.png")
    plt.savefig(output_path_2, dpi=300, bbox_inches='tight')
    output_paths.append(output_path_2)
    plt.close(fig2)

    # ===== GRAPH 3: Individual Culling Technique Effectiveness =====
    fig3, ax3 = plt.subplots(figsize=(10, 7))

    # Compare individual techniques
    individual_techniques = ['Only Orientation', 'Only View-Frustum', 'Only Distance']

    for col in individual_techniques:
        speedup = df[col] / baseline
        ax3.plot(df['Blade Count'], speedup, marker='o', linewidth=2.5,
                label=col, color=colors[col], markersize=8)

    ax3.axhline(y=1.0, color='red', linestyle='--', linewidth=2, alpha=0.5, label='Baseline (No Culling)')
    ax3.set_xscale('log', base=2)
    ax3.set_xlabel('Blade Count (log scale)', fontsize=13, fontweight='bold')
    ax3.set_ylabel('Speedup Factor', fontsize=13, fontweight='bold')
    ax3.set_title('Vulkan Grass Renderer - Individual Culling Technique Effectiveness', fontsize=14, fontweight='bold', pad=20)
    ax3.legend(loc='best', fontsize=11)
    ax3.grid(True, alpha=0.3, linestyle='--')
    ax3.set_xticks(xticks)
    ax3.set_xticklabels([f'2^{int(np.log2(x))}' for x in xticks], rotation=45)

    plt.tight_layout()
    output_path_3 = os.path.join(output_dir, f"{csv_basename}_individual_techniques.png")
    plt.savefig(output_path_3, dpi=300, bbox_inches='tight')
    output_paths.append(output_path_3)
    plt.close(fig3)

    print(f"\nGraphs saved:")
    for path in output_paths:
        print(f"  - {os.path.basename(path)}")

    # Print key insights
    print("\n=== Key Insights ===")

    # Find best performing configuration
    max_blade_count = df['Blade Count'].max()
    max_blade_row = df[df['Blade Count'] == max_blade_count].iloc[0]

    print(f"\nAt maximum blade count ({max_blade_count}):")
    for col in df.columns[1:]:
        print(f"  {col}: {max_blade_row[col]:.2f} FPS")

    # Calculate improvement
    no_culling_fps = max_blade_row['All Culling Off']
    all_culling_fps = max_blade_row['All Culling On']
    improvement = (all_culling_fps / no_culling_fps - 1) * 100

    print(f"\nCulling improvement at max blade count: {improvement:.1f}%")
    print(f"  (From {no_culling_fps:.2f} FPS to {all_culling_fps:.2f} FPS)")

    # Find most effective individual technique
    print("\nMost effective individual culling technique:")
    individual_cols = ['Only Orientation', 'Only View-Frustum', 'Only Distance']
    best_technique = max(individual_cols, key=lambda col: max_blade_row[col])
    print(f"  {best_technique}: {max_blade_row[best_technique]:.2f} FPS")
    print(f"  ({(max_blade_row[best_technique] / no_culling_fps - 1) * 100:.1f}% improvement)")

    print("\n" + "="*50)
    print("Analysis complete!")

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python analyze_performance.py <csv_file_path>")
        print("\nExample:")
        print("  python analyze_performance.py performance_test_Intel_i9_RTX5080_20251028.csv")
        sys.exit(1)

    csv_path = sys.argv[1]
    analyze_performance(csv_path)
