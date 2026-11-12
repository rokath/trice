# Handoff: Trice Publication Strategy

## Goal

Create technically useful public material about Trice while keeping the canonical content under the Trice project's control and using external platforms mainly for reach and discovery.

The strategy should support:

- Trice repository/documentation
- a project website if desired
- Medium as an optional distribution channel
- YouTube for later video publication
- conference-related publication constraints

## Canonical source

The preferred model is:

```text
Trice repo / Trice website = canonical original
Medium / other platforms   = syndicated copy or adapted version
YouTube                     = video channel
```

Do not make Medium the only home of important technical content.

Reasons:

- Trice keeps long-term control over the text.
- Links remain stable even if Medium changes policy.
- Source, examples and benchmarks can live next to the documentation.
- External platforms can still provide discovery and reach.

## Medium

Medium can be useful for Trice, but only as an additional channel.

Recommended policy:

- Do not put Trice articles behind Medium's paywall.
- Do not use Medium monetization for these articles.
- Publish the canonical version first in the Trice project or on a Trice-controlled website.
- Import or republish the article on Medium with a canonical link to the original when possible.
- Keep code and benchmark material in the repository and link to it from the article.

The purpose of Medium here is discovery, not content ownership or monetization.

## Suitable article style

Avoid a generic product-introduction article such as:

```text
Introducing Trice – an Embedded Logging Library
```

Prefer problem-oriented technical articles.

Possible themes:

- Software tracing with low target overhead
- Logging inside interrupt handlers
- Why formatting text on the target is often expensive
- Deferred decoding versus on-target decoding
- Instrumentation cost: timestamps, protection, critical sections and buffering
- Keeping generated trace IDs out of user source
- Tracing from 8-bit MCUs to larger embedded systems
- Reproducible Trice versus `printf` benchmark

The article should explain engineering trade-offs, not merely advertise features.

## Benchmark article

A particularly strong publication would accompany the reproducible benchmark project.

Suggested structure:

1. Define what is being compared.
2. Explain why a single `printf` implementation would be unfair.
3. Explain the Trice configurations under test.
4. Separate target-side work from formatting and transport.
5. Show results on at least one real MCU.
6. Show the same project on a second architecture, ideally AVR8.
7. Link to the executable benchmark project.
8. Encourage readers to change the configuration and reproduce the numbers.

Key message:

> Do not trust a hand-picked benchmark number. Run the benchmark, change the configuration and inspect the generated code yourself.

## EOC publication constraint

The relevant correspondence is with the Embedded Online Conference (EOC), not FOSDEM.

Jacob Beningo clarified by email that:

- the author retains copyright in his own material;
- EOC receives permission to use the submitted material;
- speakers are encouraged to wait about six months before publishing it themselves so as not to reduce the value of the conference.

This was phrased as an encouragement rather than a mandatory copyright transfer or strict embargo.

## Practical interpretation

### Clearly safe to continue publishing

- normal Trice documentation
- README updates
- API documentation
- implementation notes
- bug fixes
- benchmark source code
- benchmark results
- articles about individual Trice concepts or features

### Publish with some care before the six-month period ends

A substantial article is reasonable if it is not essentially a transcript or substitute for the complete EOC presentation.

Good distinction:

```text
Article = technical explanation, architecture, reproducible experiments
EOC material = complete presentation flow, selected demos, narrative, recorded session
```

### Prefer to delay

Until roughly six months after EOC publication/event, avoid independently publishing:

- the complete EOC video
- a near-identical re-recording of the EOC presentation
- the complete talk script if it reproduces the presentation almost verbatim
- a full slide-by-slide article that effectively substitutes for the conference material

## YouTube

Recommended video policy:

- Keep the original recording under local control.
- Respect EOC's requested waiting period for substantially identical video material.
- After that period, publish the project's own video on YouTube if useful.
- Link the video from the Trice repository/documentation.
- Prefer a stable Trice documentation page as the canonical context page, with YouTube as the video host.

## Publication sequence

A sensible sequence is:

```text
1. Improve canonical Trice documentation continuously.
2. Build the reproducible instrumentation benchmark.
3. Publish a technical article on the Trice-controlled site/repo.
4. Syndicate/adapt it to Medium without paywall.
5. Continue publishing narrower technical follow-up articles.
6. Publish conference-related video material after the requested waiting period.
7. Link final video, article and benchmark from the Trice documentation.
```

## Suggested first article

Working title:

```text
Software Tracing Beyond printf: Measuring the Real Cost of Instrumentation
```

Alternative:

```text
How Much Does Embedded Logging Really Cost?
```

Core content:

- several `printf` implementations
- Trice fast and typical configurations
- real hardware measurements
- simulated/reproducible targets where useful
- code size, RAM, stack and wire bandwidth
- 8-bit AVR as an additional target
- repository link with exact commands to rerun everything

This article would be useful independently of any conference and would remain valid as a long-term reference.

## Tone

Keep publications technical and falsifiable.

Prefer:

- exact configurations
- source links
- compiler versions
- disassembly
- reproducible measurements
- documented limitations

Avoid:

- universal speed claims
- presenting historic best-case numbers as typical
- weak `printf` strawman comparisons
- unsupported cycle-accuracy claims from simulators
- marketing language such as "revolutionary" or "zero overhead"

## Long-term objective

Create a small set of high-quality, durable Trice technical references rather than a high-volume blog.

Each article should ideally connect to executable source or a reproducible experiment in the repository.
