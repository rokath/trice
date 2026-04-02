---
layout: default
---

{% comment %}
This file defines the Jekyll site start page.
It includes the repository README.md and renders it as normal Markdown so the
project README becomes the homepage without duplicating content.
{% endcomment %}

{% comment %}
Capture the raw README content first, then pass it through markdownify.
This keeps the homepage content in sync with README.md.
{% endcomment %}
{% capture readme %}
{% include_relative README.md %}
{% endcapture %}
{{ readme | markdownify }}
