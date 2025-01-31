import os
import random
import argparse
from faker import Faker
from multiprocessing import Pool
import time
from tqdm import tqdm

# 初始化Faker生成器
fake = Faker()
Faker.seed(42)
random.seed(42)

# 预定义的主题关键词库
TOPIC_KEYWORDS = {
    "technology": ["computer", "software", "hardware", "network", "algorithm", "data", "cloud", "AI", "machine learning"],
    "health": ["medicine", "fitness", "nutrition", "disease", "vaccine", "hospital", "wellness", "diet"],
    "sports": ["football", "basketball", "olympics", "tournament", "athlete", "championship", "training", "coach"],
    "entertainment": ["movie", "celebrity", "music", "award", "festival", "broadcast", "streaming", "pop culture"],
    "politics": ["government", "election", "policy", "diplomacy", "legislation", "parliament", "summit", "treaty"],
    "business": ["market", "finance", "investment", "startup", "merger", "acquisition", "stock", "economy"],
    "science": ["research", "discovery", "experiment", "theory", "publication", "scientist", "laboratory", "innovation"]
}

def generate_document(doc_id, output_dir, min_length=500, max_length=2000):
    """生成单个文档"""
    # 随机选择1-3个主题
    topics = random.sample(list(TOPIC_KEYWORDS.keys()), k=random.randint(1, 3))

    # 生成文档结构
    title = fake.sentence()
    url = fake.url()
    date = fake.date_this_decade()

    # 构建内容
    content = []
    content.append(f"URL: {url}")
    content.append(f"Title: {title}")
    content.append(f"Date: {date}")
    content.append("\n")

    # 生成正文内容
    num_paragraphs = random.randint(5, 15)
    for _ in range(num_paragraphs):
        # 混合Faker生成的文本和关键词
        base_text = fake.paragraph(nb_sentences=8)

        # 插入主题关键词
        for topic in topics:
            keywords = TOPIC_KEYWORDS[topic]
            insert_pos = random.randint(0, len(base_text)//2)
            base_text = (base_text[:insert_pos]
                         + f" {random.choice(keywords)} "
                         + base_text[insert_pos:])

        content.append(base_text)

    # 添加随机外部链接
    num_links = random.randint(1, 3)
    content.append("\nRelated Links:")
    for _ in range(num_links):
        content.append(f"- {fake.url()}")

    full_text = "\n".join(content)

    # 保存文件
    subdir = os.path.join(output_dir, f"{doc_id % 1000:03d}")
    os.makedirs(subdir, exist_ok=True)
    filename = os.path.join(subdir, f"doc_{doc_id}.txt")

    with open(filename, "w", encoding="utf-8") as f:
        f.write(full_text)

def generate_batch(args):
    """多进程批处理生成"""
    start_id, num_docs, output_dir = args
    for i in range(start_id, start_id + num_docs):
        generate_document(i, output_dir)

def main():
    parser = argparse.ArgumentParser(description="Large Scale Search Dataset Generator")
    parser.add_argument("-o", "--output", default="./big_dataset",
                        help="Output directory")
    parser.add_argument("-n", "--num-docs", type=int, default=1_000_000,
                        help="Total number of documents to generate")
    parser.add_argument("-j", "--workers", type=int, default=os.cpu_count(),
                        help="Number of worker processes")
    parser.add_argument("--batch-size", type=int, default=10_000,
                        help="Documents per batch")

    args = parser.parse_args()

    print(f"Generating {args.num_docs:,} documents in {args.output}")
    print(f"Using {args.workers} workers with batch size {args.batch_size}")

    os.makedirs(args.output, exist_ok=True)

    # 准备批处理任务
    batches = []
    remaining = args.num_docs
    current_id = 0
    while remaining > 0:
        batch_size = min(args.batch_size, remaining)
        batches.append((current_id, batch_size, args.output))
        current_id += batch_size
        remaining -= batch_size

    start_time = time.time()

    # 使用多进程池
    with Pool(args.workers) as pool:
        list(tqdm(pool.imap(generate_batch, batches), total=len(batches), desc="Generating"))

    elapsed = time.time() - start_time
    print(f"\nGeneration complete in {elapsed:.2f} seconds")
    print(f"Average speed: {args.num_docs/elapsed:.1f} docs/sec")

if __name__ == "__main__":
    main()