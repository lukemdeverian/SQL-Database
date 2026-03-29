#ifndef HTML_LOGGER_H
#define HTML_LOGGER_H

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <chrono>
#include <iomanip>

using namespace std;

struct QueryResult {
    string command;
    vector<string> fields;
    vector<vector<string>> rows;
    vector<long> recnos;
    double elapsed_ms;
};

class HTMLLogger {
public:
    HTMLLogger() {}

    void log(const string& cmd,
             const vector<string>& fields,
             const vector<vector<string>>& rows,
             const vector<long>& recnos,
             double elapsed_ms) {
        QueryResult r;
        r.command    = cmd;
        r.fields     = fields;
        r.rows       = rows;
        r.recnos     = recnos;
        r.elapsed_ms = elapsed_ms;
        _results.push_back(r);
    }

    void write(const string& filename) {
        ofstream f(filename);
        f << html_head();
        f << "<body>\n";
        f << header();
        f << "<main>\n";
        for (int i = 0; i < (int)_results.size(); i++) {
            f << render_card(_results[i], i + 1);
        }
        f << "</main>\n";
        f << html_foot();
        f << "</body>\n</html>\n";
        f.close();
    }

private:
    vector<QueryResult> _results;

    string escape(const string& s) {
        string out;
        for (char c : s) {
            if      (c == '<') out += "&lt;";
            else if (c == '>') out += "&gt;";
            else if (c == '&') out += "&amp;";
            else               out += c;
        }
        return out;
    }

    string recnos_str(const vector<long>& r) {
        if (r.empty()) return "none";
        string s = "";
        for (int i = 0; i < (int)r.size(); i++) {
            if (i) s += ", ";
            s += to_string(r[i]);
        }
        return s;
    }

    string render_card(const QueryResult& r, int idx) {
        ostringstream o;

        // detect command type for badge
        string cmd_upper = r.command;
        string badge_class = "badge-select";
        string cmd_type = "SELECT";
        if (r.command.size() >= 4) {
            string prefix = r.command.substr(0, 4);
            for (auto& c : prefix) c = toupper(c);
            if (prefix == "MAKE") { badge_class = "badge-make"; cmd_type = "CREATE"; }
            else if (prefix == "INSE") { badge_class = "badge-insert"; cmd_type = "INSERT"; }
            else if (prefix == "UPDA") { badge_class = "badge-update"; cmd_type = "UPDATE"; }
            else if (prefix == "DELE") { badge_class = "badge-delete"; cmd_type = "DELETE"; }
        }

        o << "<div class='card' style='animation-delay:" << (idx * 0.08) << "s'>\n";
        o << "  <div class='card-header'>\n";
        o << "    <div class='card-left'>\n";
        o << "      <span class='query-num'>#" << idx << "</span>\n";
        o << "      <span class='badge " << badge_class << "'>" << cmd_type << "</span>\n";
        o << "    </div>\n";
        o << "    <span class='timing'>" << fixed << setprecision(2) << r.elapsed_ms << " ms</span>\n";
        o << "  </div>\n";

        // SQL query block
        o << "  <div class='sql-block'><span class='prompt'>&gt;</span> "
          << escape(r.command) << "</div>\n";

        // result table
        if (!r.fields.empty() && !r.rows.empty()) {
            o << "  <div class='table-wrap'>\n";
            o << "    <table>\n<thead><tr>\n";
            for (auto& f : r.fields)
                o << "      <th>" << escape(f) << "</th>\n";
            o << "    </tr></thead>\n<tbody>\n";
            for (auto& row : r.rows) {
                o << "    <tr>";
                for (auto& cell : row)
                    o << "<td>" << escape(cell) << "</td>";
                o << "</tr>\n";
            }
            o << "    </tbody></table>\n";
            o << "  </div>\n";
        } else if (r.fields.empty() || r.rows.empty()) {
            o << "  <div class='no-rows'>No rows returned</div>\n";
        }

        // recnos
        o << "  <div class='recnos'>";
        o << "<span class='recnos-label'>records selected</span> ";
        o << "<span class='recnos-val'>[" << recnos_str(r.recnos) << "]</span>";
        o << "</div>\n";

        o << "</div>\n";
        return o.str();
    }

    string header() {
        return R"(
<header>
  <div class="header-inner">
    <div class="logo">
      <span class="logo-bracket">{</span>
      <span class="logo-text">SQL<span class="logo-accent">Engine</span></span>
      <span class="logo-bracket">}</span>
    </div>
    <p class="subtitle">Custom C++ Database Engine &mdash; Query Execution Demo</p>
    <div class="header-tags">
      <span class="tag">B+ Tree Indexing</span>
      <span class="tag">CREATE</span>
      <span class="tag">INSERT</span>
      <span class="tag">SELECT</span>
      <span class="tag">UPDATE</span>
    </div>
  </div>
</header>
)";
    }

    string html_head() {
        return R"(<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>SQL Engine Demo</title>
<link rel="preconnect" href="https://fonts.googleapis.com">
<link href="https://fonts.googleapis.com/css2?family=JetBrains+Mono:wght@400;600;700&family=Syne:wght@400;600;800&display=swap" rel="stylesheet">
<style>
  *, *::before, *::after { box-sizing: border-box; margin: 0; padding: 0; }

  :root {
    --bg:        #0a0a0f;
    --surface:   #11111a;
    --surface2:  #1a1a28;
    --border:    #2a2a40;
    --accent:    #7b6ff0;
    --accent2:   #4fc3f7;
    --green:     #4ade80;
    --orange:    #fb923c;
    --red:       #f87171;
    --yellow:    #fbbf24;
    --text:      #e2e2f0;
    --muted:     #6b6b90;
    --mono:      'JetBrains Mono', monospace;
    --sans:      'Syne', sans-serif;
  }

  html { scroll-behavior: smooth; }

  body {
    background: var(--bg);
    color: var(--text);
    font-family: var(--sans);
    min-height: 100vh;
    background-image:
      radial-gradient(ellipse 80% 50% at 50% -20%, rgba(123,111,240,0.15), transparent),
      radial-gradient(ellipse 60% 40% at 80% 80%, rgba(79,195,247,0.08), transparent);
  }

  header {
    padding: 60px 24px 48px;
    text-align: center;
    border-bottom: 1px solid var(--border);
    position: relative;
  }
  header::after {
    content: '';
    position: absolute;
    bottom: -1px; left: 50%;
    transform: translateX(-50%);
    width: 120px; height: 2px;
    background: linear-gradient(90deg, transparent, var(--accent), transparent);
  }

  .logo {
    font-family: var(--sans);
    font-size: clamp(2.5rem, 6vw, 4.5rem);
    font-weight: 800;
    letter-spacing: -2px;
    margin-bottom: 12px;
  }
  .logo-bracket { color: var(--accent); opacity: 0.6; }
  .logo-text    { color: var(--text); }
  .logo-accent  { color: var(--accent); }

  .subtitle {
    font-family: var(--mono);
    font-size: 0.85rem;
    color: var(--muted);
    margin-bottom: 24px;
    letter-spacing: 0.05em;
  }

  .header-tags {
    display: flex;
    flex-wrap: wrap;
    gap: 8px;
    justify-content: center;
  }
  .tag {
    font-family: var(--mono);
    font-size: 0.72rem;
    padding: 4px 12px;
    border: 1px solid var(--border);
    border-radius: 999px;
    color: var(--muted);
    letter-spacing: 0.04em;
  }

  main {
    max-width: 960px;
    margin: 0 auto;
    padding: 48px 24px 80px;
    display: flex;
    flex-direction: column;
    gap: 20px;
  }

  .card {
    background: var(--surface);
    border: 1px solid var(--border);
    border-radius: 12px;
    overflow: hidden;
    opacity: 0;
    transform: translateY(16px);
    animation: rise 0.5s ease forwards;
    transition: border-color 0.2s;
  }
  .card:hover { border-color: var(--accent); }

  @keyframes rise {
    to { opacity: 1; transform: translateY(0); }
  }

  .card-header {
    display: flex;
    align-items: center;
    justify-content: space-between;
    padding: 12px 20px;
    border-bottom: 1px solid var(--border);
    background: var(--surface2);
  }
  .card-left { display: flex; align-items: center; gap: 10px; }

  .query-num {
    font-family: var(--mono);
    font-size: 0.75rem;
    color: var(--muted);
  }

  .badge {
    font-family: var(--mono);
    font-size: 0.68rem;
    font-weight: 700;
    padding: 3px 10px;
    border-radius: 4px;
    letter-spacing: 0.08em;
    text-transform: uppercase;
  }
  .badge-select { background: rgba(79,195,247,0.15); color: var(--accent2); }
  .badge-make   { background: rgba(74,222,128,0.15); color: var(--green);   }
  .badge-insert { background: rgba(251,191,36,0.15);  color: var(--yellow); }
  .badge-update { background: rgba(251,146,60,0.15);  color: var(--orange); }
  .badge-delete { background: rgba(248,113,113,0.15); color: var(--red);    }

  .timing {
    font-family: var(--mono);
    font-size: 0.72rem;
    color: var(--muted);
  }

  .sql-block {
    font-family: var(--mono);
    font-size: 0.88rem;
    padding: 16px 20px;
    color: var(--text);
    background: rgba(0,0,0,0.2);
    border-bottom: 1px solid var(--border);
    white-space: pre-wrap;
    word-break: break-all;
  }
  .prompt { color: var(--accent); margin-right: 8px; font-weight: 700; }

  .table-wrap {
    overflow-x: auto;
    padding: 16px 20px;
  }
  table {
    width: 100%;
    border-collapse: collapse;
    font-family: var(--mono);
    font-size: 0.82rem;
  }
  thead tr { border-bottom: 1px solid var(--border); }
  th {
    text-align: left;
    padding: 8px 16px 10px;
    color: var(--accent);
    font-weight: 600;
    letter-spacing: 0.05em;
    white-space: nowrap;
  }
  td {
    padding: 9px 16px;
    color: var(--text);
    border-bottom: 1px solid rgba(255,255,255,0.03);
  }
  tbody tr:last-child td { border-bottom: none; }
  tbody tr:hover td { background: rgba(123,111,240,0.06); }

  .no-rows {
    font-family: var(--mono);
    font-size: 0.8rem;
    color: var(--muted);
    padding: 16px 20px;
    font-style: italic;
  }

  .recnos {
    padding: 10px 20px;
    font-family: var(--mono);
    font-size: 0.78rem;
    border-top: 1px solid var(--border);
    background: var(--surface2);
    display: flex;
    gap: 10px;
    align-items: center;
  }
  .recnos-label { color: var(--muted); }
  .recnos-val   { color: var(--green); }
</style>
</head>
)";
    }

    string html_foot() {
        return R"(
<footer style="text-align:center;padding:32px;font-family:var(--mono);font-size:0.75rem;color:var(--muted);border-top:1px solid var(--border);">
  Generated by SQL Engine &mdash; Custom C++ Database Implementation
</footer>
)";
    }
};

#endif
